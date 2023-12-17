//
// Created by Elliot Nash on 2023-12-06.
//

#ifndef AOCLIB_H
#define AOCLIB_H

#include "aoclist.h"
#include "aocheap.h"
#include "aocmap.h"

void print_int(const int* value);
void print_char(const char* value);
void print_uint64(const uint64_t* value);
void print_int64(const int64_t* value);
void print_string(const char* string);

int atoin(const char* start, const char* end);
int* atoin_alloc(const char* start, const char* end);

uint64_t atouln(const char* start, const char* end);
uint64_t* atouln_alloc(const char* start, const char* end);

int64_t atoln(const char* start, const char* end);
int64_t* atoln_alloc(const char* start, const char* end);

arraylist_t* nums_in_line(const char* line, void* (*atonn)(const char*, const char*));

uint64_t line_as_uint64(const char* line);

uint64_t gcd(uint64_t lhs, uint64_t rhs);
uint64_t lcm(uint64_t lhs, uint64_t rhs);

#endif AOCLIB_H
