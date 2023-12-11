//
// Created by Elliot Nash on 2023-12-09.
//

#include "aocmap.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Only for odd numbers
static bool is_prime(size_t n) {
    const size_t max = sqrt(n);
    for (int i = 3; i < max; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

// Only for odd numbers
static size_t next_prime(const size_t n) {
    if (is_prime(n)) {
        return n;
    }
    return next_prime(n + 2);
}

size_t hash_char(const char* c) {
    return *c;
}

size_t hash_int(const int* n) {
    return *n;
}

size_t hash_uint64(const uint64_t* n) {
    return *n;
}

size_t hash_string(const char* string) {
    size_t hash = 0;
    for (int i = 0; string[i] != '\0'; ++i) {
        hash = 31*hash + string[i];
    }
    return hash;
}

hash_map_t* hash_map_create(const hash_code_func hash_code, size_t initial_capacity, const float load_factor) {
    if (initial_capacity % 2 == 0) {
        ++initial_capacity;
    }
    initial_capacity = next_prime(initial_capacity);

    hash_map_t* map = malloc(sizeof(hash_map_t));

    map->hash_code = hash_code;
    map->h2_prime = next_prime(initial_capacity/2);
    map->capacity = initial_capacity;
    map->load_factor = load_factor;
    map->data = malloc(sizeof(hash_entry_t) * initial_capacity);
    // zero memory
    memset(map->data, 0, sizeof(hash_entry_t) * initial_capacity);

    return map;
}

// TODO write rehash function (and make sure to call it when growing the array)


static void ensure_capacity(hash_map_t* map) {
    if (map->count >= map->capacity * map->load_factor) {
        // Use current capacity as second hash prime.
        map->h2_prime = map->capacity;
        map->capacity = next_prime(map->capacity * 2 + 1);

        hash_entry_t* old_data = map->data;
        map->data = malloc(sizeof(hash_entry_t) * map->capacity);
        memset(map->data, 0, sizeof(hash_entry_t) * map->capacity);

        // Copy data (rehashing)
        for (int i = 0; i < map->h2_prime; ++i) {
            const hash_entry_t* entry = old_data + i;
            if (entry->status == HASH_ENTRY_PRESENT) {
                hash_map_put(map, entry->key, entry->value);
            }
        }

        free(old_data);
    }
}

void hash_map_put(hash_map_t* map, void* key, void* value) {
    ensure_capacity(map);
    const size_t hash_code = map->hash_code(key);
    const size_t h1 = abs(hash_code % map->capacity);
    const size_t h2 = abs(map->h2_prime - hash_code % map->h2_prime);
    for (int i = h1;; i += h2) {
        hash_entry_t* entry = map->data + i;
        if (entry->status == HASH_ENTRY_EMPTY || entry->status == HASH_ENTRY_REMOVED) {
            // Then we have empty spot so we should insert the entry
            entry->status = HASH_ENTRY_PRESENT;
            entry->key = key;
            entry->value = value;
            ++map->count;
            break;
        }
        if (map->hash_code(entry->key) == hash_code) {
            // Then the keys are equal so we should update the value
            entry->value = value;
            break;
        }
    }
}

void* hash_map_get(const hash_map_t* map, const void* key) {
    const size_t hash_code = map->hash_code(key);
    const size_t h1 = abs(hash_code % map->capacity);
    const size_t h2 = abs(map->h2_prime - hash_code % map->h2_prime);
    for (int i = h1;; i += h2) {
        const hash_entry_t* entry = map->data + i;
        if (entry->status == HASH_ENTRY_EMPTY) {
            return NULL;
        }
        if (entry->status == HASH_ENTRY_PRESENT && map->hash_code(entry->key) == hash_code) {
            return entry->value;
        }
    }
}

bool hash_map_contains(const hash_map_t* map, const void* key) {
    return hash_map_get(map, key) != NULL;
}

void hash_map_print(const hash_map_t* map, void (*print_key)(void*), void (*print_value)(void*)) {
    if (map->count == 0) {
        printf("{}\n");
        return;
    }
    printf("{");
    for (int i = 0; i < map->capacity; ++i) {
        const hash_entry_t* entry = map->data + i;
        if (entry->status == HASH_ENTRY_PRESENT) {
            print_key(entry->key);
            printf(":");
            print_value(entry->value);
            printf(", ");
        }
    }
    printf("\b\b}\n");
}

arraylist_t* hash_map_entry_list(const hash_map_t* map) {
    arraylist_t* out = arraylist_create(map->count);
    for (int i = 0; i < map->capacity; ++i) {
        hash_entry_t* entry = map->data + i;
        if (entry->status == HASH_ENTRY_PRESENT) {
            arraylist_add(out, entry);
        }
    }
    return out;
}

void hash_map_free(hash_map_t* map) {
    free(map->data);
    free(map);
}
