#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
    struct node* children;
    int size;
    char key;
    int value;
} node;

node root;

node* add_node(node* parent, const char key, const char value) {
    for (int i = 0; i < parent->size; ++i) {
        if (parent->children[i].key == key) {
            parent->children[key].value = value;
            return &parent->children[key];
        }
    }
    // Increase size of array by 1 (this is not effecient)
    parent->children = realloc(parent->children, sizeof(node) * parent->size+1);
    parent->children[parent->size].key = key;
    parent->children[parent->size].value = value;
    return &parent->children[parent->size++];
}

void tree_insert(char* key, int value) {
    printf("This called with key = %s, value = %d\n", key, value);
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

    // // Add one
    // const node* child = add_node(root, 0, 1, 'o', -1);
    // child = add_node(child, 0, 1, 'n', -1);
    // add_node(child, 0, 1, 'e', 1);
    //
    // // Add two and three
    // child = add_node(root, 1, 2, 't', -1);
    // child = add_node(child, 0, 1, 'w', -1);
    // add_node(child, 0, 1, 'o', 2);
    //
    // child = &root->children[1];
    // child = add_node(child, 1, 1, 'h', -1);
    // child = add_node(child, 0, 1, 'r', -1)
}

int search_number(char* string, int* out) {
    node* n = &root;
    for (int i = 0; string[i] != '\0'; ++i) {
        for (int j = 0; j < n->size; ++j) {
            if (n->children[j].key == string[i]) {
                // We found
            }
        }
    }
}

int main() {
    bool found = true;

    initialize_tree();

    FILE* fp = fopen("input.txt", "r");;

    if (fp == NULL)
        return EXIT_FAILURE;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int sum = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
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
                ++i;
                // if
            }
        }
        sum += first*10 + last;
    }

    printf("Sum is %d", sum);

    fclose(fp);
    if (line)
        free(line);
    return EXIT_SUCCESS;
}
