//
// Created by Elliot Nash on 2023-12-08.
//

#ifndef AOCHEAP_H
#define AOCHEAP_H

#include <stddef.h>
#include <stdint.h>

typedef int (*compare_func)(const void*, const void*);

typedef struct {
    compare_func compare;
    size_t capacity;
    size_t count;
    void** data;
} heap_t;

int compare_int(const int* lhs, const int* rhs);
int compare_uint64(const uint64_t* lhs, const uint64_t* rhs);

// Max heap by default, make compare return > 0 if first is smaller to make min heap.
heap_t* heap_create(compare_func compare, size_t initial_capacity);

void heap_insert(heap_t* heap, void* data);

void* heap_remove(heap_t* heap);

void heap_print(const heap_t* heap, void (*print_data)(void*));

void heap_free(heap_t* heap);

#endif //AOCHEAP_H
