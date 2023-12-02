#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct node {
    struct node* child;
    struct node* sibling;
    char key;
    int value;
} node;

node root;

node* add_node(node* parent, const char key, const int value) {
    node* n = parent->child;
    node* last = NULL;
    while (n) {
        last = n;
        if (n->key == key) {
            n->value = value;
            return n;
        }
        n = n->sibling;
    }
    // The key doesn't exist so we need to create it
    n = malloc(sizeof(node));
    // If siblings are not empty we need to link it to the last sibling
    // Otherwise link to parent
    if (last) {
        last->sibling = n;
    } else {
        parent->child = n;
    }
    n->key = key;
    n->value = value;
    return n;
}

void tree_insert(const char* key, const int value) {
    const int len = strlen(key);
    node* child = &root;
    for (int i = 0; i < len; ++i) {
        if (i != len-1) {
            child = add_node(child, key[i], -1);
        } else {
            child = add_node(child, key[i], value);
        }
    }
}

void initialize_tree() {
    tree_insert("one", 1);
    tree_insert("two", 2);
    tree_insert("three", 3);
    tree_insert("four", 4);
    tree_insert("five", 5);
    tree_insert("six", 6);
    tree_insert("seven", 7);
    tree_insert("eight", 8);
    tree_insert("nine", 9);
}

node* get_child_node(const node* parent, const char key) {
    node* n = parent->child;
    while (n) {
        if (n->key == key) {
            return n;
        }
        n = n->sibling;
    }
    return NULL;
}

int search_number(const char* string, int* number_out) {
    const node* n = &root;
    const int len = strlen(string);

    for (int i = 0; i < len; ++i) {
        n = get_child_node(n, string[i]);
        if (!n) {
            break;
        }
        if (n->value >= 0) {
            *number_out = n->value;
            return i+1;
        }
    }

    *number_out = -1;
    return 1;
}

int main() {
    initialize_tree();

    FILE* fp = fopen("input.txt", "r");;

    if (fp == NULL)
        return EXIT_FAILURE;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int part1Sum = 0;
    int part2Sum = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        {
            // Part 1
            int first = -1;
            int last = -1;
            for (int i = 0; i < read; ++i) {
                if (isnumber(line[i])) {
                    if (first == -1) {
                        first = line[i] - '0';
                    }
                    last = line[i] - '0';
                }
            }
            part1Sum += first*10 + last;
        }
        {
            int first = -1;
            int last = -1;
            for (int i = 0; i < read;) {
                if (isnumber(line[i])) {
                    if (first == -1) {
                        first = line[i] - '0';
                    }
                    last = line[i] - '0';
                    ++i;
                } else {
                    int num;
                    i += search_number(&line[i], &num);
                    if (num > 0) {
                        if (first == -1) {
                            first = num;
                        }
                        last = num;
                    }
                }
            }
            part2Sum += first*10 + last;
        }
    }

    printf("Sum in part 1 is %d\n", part1Sum);
    printf("Sum in part 2 is %d\n", part2Sum);

    fclose(fp);
    if (line)
        free(line);
    return EXIT_SUCCESS;
}
