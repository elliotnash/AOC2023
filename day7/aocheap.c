//
// Created by Elliot Nash on 2023-12-08.
//

#include "aocheap.h"

#include <stdio.h>
#include <stdlib.h>

int compare_int(const int* lhs, const int* rhs) {
    return *lhs - *rhs;
}

int compare_uint64(const uint64_t* lhs, const uint64_t* rhs) {
    if (*lhs == *rhs) {
        return 0;
    }
    if (*lhs > *rhs) {
        return 1;
    }
    return -1;
}

// Max heap by default, make compare return > 0 if first is smaller to make min heap.
heap_t* heap_create(const compare_func compare, const size_t initial_capacity) {
    heap_t* heap = malloc(sizeof(heap_t));
    heap->compare = compare;
    heap->capacity = initial_capacity;
    heap->data = malloc(sizeof(void*) * initial_capacity);
    return heap;
}

static size_t parent_index(const size_t index) {
    return (index - 1) / 2;
}

static size_t left_index(const size_t index) {
    return index * 2 + 1;
}

static size_t right_index(const size_t index) {
    return index * 2 + 2;
}

static void bubble_up(heap_t* heap, const size_t index) {
    if (index == 0) {
        // Then we're the root.
        return;
    }
    const size_t parent = parent_index(index);
    if (heap->compare(heap->data[index], heap->data[parent]) <= 0) {
        // Then the child is smaller or equal to the parent so we are done.
        return;
    }
    // Otherwise we need to swap the child and parent.
    void* temp = heap->data[index];
    heap->data[index] = heap->data[parent];
    heap->data[parent] = temp;

    bubble_up(heap, parent);
}

static void bubble_down(heap_t* heap, const size_t index) {
    if (index >= heap->count) {
        // We hit the end of the array
        return;
    }
    const size_t left = left_index(index);
    const size_t right = right_index(index);
    if (left >= heap->count) {
        // Both the children don't exist
        return;
    }
    const size_t child = right >= heap->count ?
        left :
        heap->compare(heap->data[left], heap->data[right]) >= 0 ?
            left :
            right;
    if (heap->compare(heap->data[index], heap->data[child]) >= 0) {
        // Then the parent is bigger than or equal to the child so we are done.
        return;
    }
    // Otherwise we need to swap the child and parent.
    void* temp = heap->data[index];
    heap->data[index] = heap->data[child];
    heap->data[child] = temp;

    bubble_down(heap, child);
}

void heap_insert(heap_t* heap, void* data) {
    if (heap->count >= heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(void*) * heap->capacity);
    }
    heap->data[heap->count] = data;
    bubble_up(heap, heap->count++);
}

void* heap_remove(heap_t* heap) {
    void* data = heap->data[0];
    heap->data[0] = heap->data[--heap->count];
    bubble_down(heap, 0);
    return data;
}

void heap_print(const heap_t* heap, void (*print_data)(void*)) {
    if (heap->count == 0) {
        printf("[]\n");
        return;
    }
    printf("[");
    for (int i = 0; i < heap->count; ++i) {
        print_data(heap->data[i]);
        printf(", ");
    }
    printf("\b\b]\n");
}

void heap_free(heap_t* heap) {
    free(heap->data);
    free(heap);
}
