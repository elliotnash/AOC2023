#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "aoclib.h"

void print_lines(const arraylist_t* line) {
    arraylist_print(line, print_int64);
}

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }

    int64_t part1ans = 0;
    int64_t part2ans = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        arraylist_t* root = nums_in_line(line, atoln_alloc);

        // Build reverse tree
        arraylist_t* lines = arraylist_create(50);
        arraylist_add(lines, root);
        for (int i = 0; true; ++i) {
            arraylist_t* last = lines->data[i];
            arraylist_t* next = arraylist_create(last->count);
            arraylist_add(lines, next);

            bool all_zero = true;
            for (int j = 0; j < last->count-1; ++j) {
                int64_t* diff = malloc(sizeof(int64_t));
                *diff = *(int64_t*)last->data[j+1] - *(int64_t*)last->data[j];
                if (*diff != 0) {
                    all_zero = false;
                }
                arraylist_add(next, diff);
            }
            if (all_zero) {
                break;
            }
        }

        // arraylist_print(lines, print_lines);

        int64_t* zero = malloc(sizeof(int64_t));
        *zero = 0;
        arraylist_add(lines->data[lines->count-1], zero);

        for (int i = lines->count-1; i > 0; --i) {
            arraylist_t* last = lines->data[i];
            arraylist_t* next = lines->data[i-1];

            // Part 1
            {
                int64_t* next_ell = malloc(sizeof(int64_t));
                *next_ell = *(int64_t*) last->data[last->count-1] + *(int64_t*) next->data[next->count-1];
                arraylist_add(next, next_ell);
            }
            // Part 2
            {
                int64_t* next_ell = malloc(sizeof(int64_t));
                *next_ell = *(int64_t*) next->data[0] - *(int64_t*) last->data[0];
                arraylist_add_front(next, next_ell);
            }
        }

        part1ans += *(int64_t*)root->data[root->count-1];
        part2ans += *(int64_t*)root->data[0];

        // Free memory
        for (int i = 0; i < lines->count; ++i) {
            arraylist_t* l = lines->data[i];
            for (int j = 0; j < l->count; ++j) {
                free(l->data[j]);
            }
            arraylist_free(l);
        }
        arraylist_free(lines);
    }

    printf("Part 1 answer %llu\n", part1ans);
    printf("Part 2 answer %llu\n", part2ans);

    fclose(fp);
    free(line);

    return EXIT_SUCCESS;
}
