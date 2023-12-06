#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "aoclib.h"

typedef struct {
    uint64_t destination;
    uint64_t source;
    uint64_t range;
} seed_map_t;

void print_seed_map(const seed_map_t* map) {
    printf("{%llu,%llu,%llu}", map->destination, map->source, map->range);
}

void print_seed_map_full(const seed_map_t* map) {
    printf("{destination: %llu, source: %llu, range: %llu}", map->destination, map->source, map->range);
}

bool in_seed_map(const seed_map_t* map, const uint64_t value) {
    return value >= map->source && value < map->source+map->range;
}

uint64_t map_seed(const seed_map_t* map, const uint64_t value) {
    return map->destination + (value - map->source);
}

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }


    int part1ans = 0;
    int part2ans = 0;

    arraylist_t* maps = arraylist_create(10);

    arraylist_t* seeds = NULL;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        if (strncmp(line, "seeds: ", 7) == 0) {
            seeds = nums_in_line(line+7, atouln_alloc);
        } else if (strcmp(line, "\n") == 0) {
            arraylist_t* map = arraylist_create(100);
            arraylist_add(maps, map);
        } else {
            arraylist_t* line_array = nums_in_line(line, atouln_alloc);
            if (line_array->count == 3) {
                seed_map_t* map = malloc(sizeof(seed_map_t));
                map->destination = *(uint64_t*) line_array->data[0];
                map->source = *(uint64_t*) line_array->data[1];
                map->range = *(uint64_t*) line_array->data[2];
                arraylist_t* map_array = maps->data[maps->count-1];
                arraylist_add(map_array, map);
            }
            for (int i = 0; i < line_array->count; ++i) {
                free(line_array->data[i]);
            }
            arraylist_free(line_array);
        }
    }

    // Part 1
    for (int i = 0; i < seeds->count; ++i) {
        uint64_t seed = *(uint64_t*) seeds->data[i];
        // Loop through each stage
        for (int j = 0; j < maps->count; ++j) {
            const arraylist_t* map = maps->data[j];
            // Loop through each map and if one of them applies, map it
            for (int k = 0; k < map->count; ++k) {
                const seed_map_t* sm = map->data[k];
                if (in_seed_map(sm, seed)) {
                    seed = map_seed(sm, seed);
                    break;
                }
            }
        }
        // Now we have the fully mapped seed
        if (part1ans == 0 || seed < part1ans) {
            part1ans = seed;
        }
    }

    // Part 2 (this takes a really long time)
    for (int i = 0; i < seeds->count; ++i) {
        const uint64_t start_seed = *(uint64_t*) seeds->data[i++];
        const uint64_t seed_range = *(uint64_t*) seeds->data[i];
        for (uint64_t r = 0; r < seed_range; ++r) {
            uint64_t seed = start_seed + r;
            // Loop through each stage
            for (int j = 0; j < maps->count; ++j) {
                const arraylist_t* map = maps->data[j];
                // Loop through each map and if one of them applies, map it
                for (int k = 0; k < map->count; ++k) {
                    const seed_map_t* sm = map->data[k];
                    if (in_seed_map(sm, seed)) {
                        seed = map_seed(sm, seed);
                        break;
                    }
                }
            }
            // Now we have the fully mapped seed
            if (part2ans == 0 || seed < part2ans) {
                part2ans = seed;
            }
        }
    }

    printf("Part 1 answer %d\n", part1ans);
    printf("Part 2 answer %d\n", part2ans);

    fclose(fp);
    free(line);
    arraylist_free(maps);

    // Free seeds
    for (int i = 0; i < seeds->count; ++i) {
        free(seeds->data[i]);
    }
    arraylist_free(seeds);

    return EXIT_SUCCESS;
}
