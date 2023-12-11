//
// Created by Elliot Nash on 2023-12-10.
//

#include "aoclist.h"

#include <stdio.h>
#include <stdlib.h>

arraylist_t* arraylist_create(const size_t initial_capacity) {
    arraylist_t* list = malloc(sizeof(arraylist_t));
    list->capacity = initial_capacity;
    list->data = malloc(sizeof(void*) * initial_capacity);
    return list;
}

void arraylist_add(arraylist_t* list, void* data) {
    // If we have filled the list, then we need to grow it
    if (list->count == list->capacity) {
        list->capacity*=2;
        list->data = realloc(list->data, sizeof(void*) * list->capacity);
    }
    list->data[list->count++] = data;
}

arraylist_t* arraylist_concat(const arraylist_t* lhs, const arraylist_t* rhs) {
    const size_t cum_count = lhs->count + rhs->count;
    const size_t cum_capacity = lhs->capacity + rhs->capacity;
    const size_t new_capacity = cum_count > cum_capacity ? cum_count : cum_capacity;
    arraylist_t* out = arraylist_create(new_capacity);
    for (int i = 0; i < lhs->count; ++i) {
        out->data[out->count++] = lhs->data[i];
    }
    for (int i = 0; i < rhs->count; ++i) {
        out->data[out->count++] = rhs->data[i];
    }
    return out;
}

void arraylist_print(const arraylist_t* list, void (*print_data)(void*)) {
    if (list->count == 0) {
        printf("[]\n");
        return;
    }
    printf("[");
    for (int i = 0; i < list->count; ++i) {
        print_data(list->data[i]);
        printf(", ");
    }
    printf("\b\b]\n");
}

void arraylist_free(arraylist_t* list) {
    free(list->data);
    free(list);
}
