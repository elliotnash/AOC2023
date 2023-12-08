#include "aoclib.h"
#include <stdio.h>
#include <ctype.h>

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

void print_int(const int* value) {
    printf("%d", *value);
}

void print_char(const char* value) {
    printf("%c", *value);
}

void print_uint64(const uint64_t* value) {
    printf("%llu", *value);
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

int atoin(const char* start, const char* end) {
    int out = 0;
    int place = 1;
    while (start-1 != end) {
        out += (*end - '0') * place;
        --end;
        place *= 10;
    }
    return out;
}

int* atoin_alloc(const char* start, const char* end) {
    int* num = malloc(sizeof(int));
    *num = atoin(start, end);
    return num;
}

uint64_t atouln(const char* start, const char* end) {
    uint64_t out = 0;
    uint64_t place = 1;
    while (start-1 != end) {
        out += (*end - '0') * place;
        --end;
        place *= 10;
    }
    return out;
}

uint64_t* atouln_alloc(const char* start, const char* end) {
    uint64_t* num = malloc(sizeof(uint64_t));
    *num = atouln(start, end);
    return num;
}

arraylist_t* nums_in_line(const char* line, void* (*atonn)(const char*, const char*)) {
    arraylist_t* list = arraylist_create(50);
    while (*line != '\n' && *line != '\0') {
        if (isnumber(*line)) {
            const char* start = line;
            const char* end = line+1;
            while (isnumber(*end)) {
                ++end;
            }
            --end;
            void* num = atonn(start, end);
            arraylist_add(list, num);
            line = end;
        }
        ++line;
    }
    return list;
}

uint64_t line_as_uint64(const char* line) {
    // Move pointer to end of line
    const char* end = line;
    while (*++end != '\0') {}
    --end;

    uint64_t out = 0;
    uint64_t place = 1;
    while (end != line-1) {
        if (isnumber(*end)) {
            out += (*end - '0') * place;
            place *= 10;
        }
        --end;
    }
    return out;
}
