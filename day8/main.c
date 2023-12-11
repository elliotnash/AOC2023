#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "aoclib.h"

typedef struct {
    char* left;
    char* right;
} node_t;

void print_node(const node_t* node) {
    printf("{L:\"%s\", R:\"%s\"}", node->left, node->right);
}

void free_node(node_t* node) {
    free(node->left);
    free(node->right);
    free(node);
}

void free_string(char* string) {
    free(string);
}

void node_hash_map_put(hash_map_t* map, char* key, const node_t value) {
    node_t* n = malloc(sizeof(node_t));
    *n = value;
    hash_map_put(map, key, n);
}

node_t* node_hash_map_get(const hash_map_t* map, const char* key) {
    return hash_map_get(map, key);
}

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }

    hash_map_t* nodes = hash_map_create(free_string, free_string, hash_string, 2000, 0.5);

    uint64_t part1ans = 0;
    uint64_t part2ans = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    // Read instructions
    char* instructions = NULL;
    read = getline(&line, &len, fp);
    *(line+read-1) = '\0';
    instructions = malloc(sizeof(char) * read-1);
    strcpy(instructions, line);
    getline(&line, &len, fp);

    while ((read = getline(&line, &len, fp)) != -1) {
        char* key = malloc(sizeof(char) * 3);
        strncpy(key, line, 3);

        char* left = malloc(sizeof(char) * 3);
        strncpy(left, line+7, 3);

        char* right = malloc(sizeof(char) * 3);
        strncpy(right, line+12, 3);

        node_hash_map_put(nodes, key, (node_t) {left, right});
    }

    printf("Instructions: %s\n", instructions);

    // Part 1
    // hash_map_print(nodes, print_string, print_node);
    char* key = "AAA";
    const char* instruction = instructions;
    while (strcmp(key, "ZZZ") != 0) {
        if (*instruction == '\0') {
            instruction = instructions;
        }
        node_t* node = node_hash_map_get(nodes, key);
        if (*instruction == 'L') {
            key = node->left;
        } else {
            key = node->right;
        }
        ++part1ans;
        ++instruction;
    }

    printf("Part 1 answer %llu\n", part1ans);
    printf("Part 2 answer %llu\n", part2ans);

    fclose(fp);
    free(line);

    hash_map_free(nodes);

    return EXIT_SUCCESS;
}
