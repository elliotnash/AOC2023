//
// Created by Elliot Nash on 2023-12-06.
//

#include "aoclib.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void print_int(const int* value) {
    printf("%d", *value);
}

void print_char(const char* value) {
    printf("'%c'", *value);
}

void print_string(const char* string) {
    printf("\"%s\"", string);
}

void print_uint64(const uint64_t* value) {
    printf("%llu", *value);
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

uint64_t gcd(const uint64_t lhs, const uint64_t rhs) {
    const uint64_t remainder = lhs % rhs;

    if (remainder == 0) {
        return rhs;
    }

    return gcd(rhs, remainder);
}

uint64_t lcm(const uint64_t lhs, const uint64_t rhs) {
    return lhs / gcd(lhs, rhs) * rhs;
}
