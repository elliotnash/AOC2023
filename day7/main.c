#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aoclib.h"

typedef int (*compare_func)(const void*, const void*);

typedef struct {
    compare_func compare;
    size_t capacity;
    void** data;
} heap_t;

heap_t* heap_create(const compare_func compare, const size_t initial_capacity) {
    heap_t* heap = malloc(sizeof(heap_t));
    heap->compare = compare;
    heap->capacity = initial_capacity;
    heap->data = malloc(sizeof(void*) * initial_capacity);
    return heap;
}

void heap_insert(heap_t* heap, void* data) {

}

void arraylist_add(arraylist_t* list, void* data) {
    // If we have filled the list, then we need to grow it
    if (list->count == list->capacity) {
        list->capacity*=2;
        list->data = realloc(list->data, sizeof(void*) * list->capacity);
    }
    list->data[list->count++] = data;
}

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }


    uint64_t part1ans = 0;
    uint64_t part2ans = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {

    }

    printf("Part 1 answer %d\n", part1ans);
    printf("Part 2 answer %llu\n", part2ans);

    fclose(fp);
    free(line);

    return EXIT_SUCCESS;
}
