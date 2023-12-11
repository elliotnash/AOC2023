//
// Created by Elliot Nash on 2023-12-09.
//

#ifndef AOCMAP_H
#define AOCMAP_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "aoclist.h"

typedef size_t (*hash_code_func)(const void*);

typedef enum {
    HASH_ENTRY_EMPTY,
    HASH_ENTRY_PRESENT,
    HASH_ENTRY_REMOVED
} hash_entry_status_t;

typedef struct {
    hash_entry_status_t status;
    void* key;
    void* value;
} hash_entry_t;

typedef struct {
    hash_code_func hash_code;
    size_t h2_prime;
    size_t capacity;
    float load_factor;
    size_t count;
    hash_entry_t* data;
} hash_map_t;

size_t hash_char(const char* c);

size_t hash_int(const int* n);

size_t hash_uint64(const uint64_t* n);

size_t hash_string(const char* string);

hash_map_t* hash_map_create(hash_code_func hash_code, size_t initial_capacity, float load_factor);

// TODO write rehash function

// TODO make free function passed to create and call it when overwriting value (free value)
void hash_map_put(hash_map_t* map, void* key, void* value);

void* hash_map_get(const hash_map_t* map, const void* key);

bool hash_map_contains(const hash_map_t* map, const void* key);

void hash_map_print(const hash_map_t* map, void (*print_key)(void*), void (*print_value)(void*));

arraylist_t* hash_map_entry_list(const hash_map_t* map);

void hash_map_free(hash_map_t* map);

#endif //AOCMAP_H
