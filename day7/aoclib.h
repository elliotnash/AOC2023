#ifndef AOCLIB_H
#define AOCLIB_H

#include <stdlib.h>

typedef struct {
    size_t capacity;
    size_t count;
    void** data;
} arraylist_t;

arraylist_t* arraylist_create(const size_t initial_capacity);

void arraylist_add(arraylist_t* list, void* data);

arraylist_t* arraylist_concat(const arraylist_t* lhs, const arraylist_t* rhs);

void print_int(const int* value);
void print_char(const char* value);
void print_uint64(const uint64_t* value);

void arraylist_print(const arraylist_t* list, void (*print_data)(void*));

// Data must be free'd first
void arraylist_free(arraylist_t* list);

int atoin(const char* start, const char* end);
int* atoin_alloc(const char* start, const char* end);

uint64_t atouln(const char* start, const char* end);
uint64_t* atouln_alloc(const char* start, const char* end);

arraylist_t* nums_in_line(const char* line, void* (*atonn)(const char*, const char*));

uint64_t line_as_uint64(const char* line);

#endif AOCLIB_H
