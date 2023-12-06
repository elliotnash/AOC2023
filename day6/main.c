#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    uint64_t duration;
    uint64_t distance;
} race_t;

void print_race(const race_t* race) {
    printf("{%llu,%llu}", race->duration, race->distance);
}

#include "aoclib.h"

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }


    int part1ans = 1;
    uint64_t part2ans = 0;

    arraylist_t* times = arraylist_create(2);
    race_t* part2race = malloc(sizeof(race_t));

    char* line = NULL;
    int line_num = 0;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        arraylist_add(times, nums_in_line(line, atouln_alloc));
        if (line_num++ == 0) {
            printf("about to do it\n");
            part2race->duration = line_as_uint64(line);
            printf("done\n");
        } else {
            part2race->distance = line_as_uint64(line);
        }
    }

    // Part 1
    for (int i = 0; i < ((arraylist_t*) times->data[0])->count; ++i) {
        race_t* race = malloc(sizeof(race_t));
        race->duration = *(uint64_t*) ((arraylist_t*) times->data[0])->data[i];
        race->distance = *(uint64_t*) ((arraylist_t*) times->data[1])->data[i];

        free(((arraylist_t*) times->data[0])->data[i]);
        free(((arraylist_t*) times->data[1])->data[i]);

        int sum = 0;
        for (int j = 0; j < race->duration; ++j) {
            const int distance = (race->duration - j) * j;
            if (distance > race->distance) {
                sum++;
            }
        }
        part1ans *= sum;

        free(race);
    }
    arraylist_free(times);

    // Part 2
    for (uint64_t j = 0; j < part2race->duration; ++j) {
        const uint64_t distance = (part2race->duration - j) * j;
        if (distance > part2race->distance) {
            part2ans++;
        }
    }
    free(part2race);

    printf("Part 1 answer %d\n", part1ans);
    printf("Part 2 answer %llu\n", part2ans);

    fclose(fp);
    free(line);

    return EXIT_SUCCESS;
}
