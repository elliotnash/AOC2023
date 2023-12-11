//
// Created by Elliot Nash on 2023-12-10.
//

#ifndef AOCLIST_H
#define AOCLIST_H

#include <stddef.h>

typedef struct {
    size_t capacity;
    size_t count;
    void** data;
} arraylist_t;

arraylist_t* arraylist_create(size_t initial_capacity);

void arraylist_add(arraylist_t* list, void* data);

arraylist_t* arraylist_concat(const arraylist_t* lhs, const arraylist_t* rhs);

void arraylist_print(const arraylist_t* list, void (*print_data)(void*));

// Data must be free'd first
void arraylist_free(arraylist_t* list);

#endif //AOCLIST_H
