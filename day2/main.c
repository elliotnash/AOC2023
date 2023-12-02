#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef struct set {
    int red;
    int green;
    int blue;
    struct set* next;
} set_t;

typedef struct {
    int id;
    set_t* set;
} game_t;

bool game_is_possible(const game_t* game) {
    const set_t* set = game->set;
    while (set) {
        if (set->red > 12 || set->green > 13 || set->blue > 14) {
            return false;
        }
        set = set->next;
    }
    return true;
}

int main() {
    FILE* fp = fopen("input.txt", "r");;

    if (fp == NULL)
        return EXIT_FAILURE;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int part1sum = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        game_t* game = malloc(sizeof(game_t));

        // Parse input
        {
            // Remove newline char
            char* end = line + read;
            if (*(end-1) == '\n') {
                *--end = '\0';
                --read;
            }

            char* id_start = strchr(line, ' ');
            char* id_end = strchr(line, ':');
            *id_end = '\0';
            game->id = atoi(id_start);

            set_t* set = NULL;

            char* sets = id_end+1;

            char* set_start = sets;
            char* set_end = strchr(sets, ';');
            char* sets_end = line+read;
            while (set_start != sets_end+1) {
                // Allocate set
                if (set) {
                    set->next = malloc(sizeof(set_t));
                    set = set->next;
                } else {
                    game->set = malloc(sizeof(set_t));
                    set = game->set;
                }

                // Split strings
                *set_end = '\0';

                const char* cubes = strtok(set_start, ",");
                while (cubes) {
                    char* str_start = strchr(cubes+1, ' ');
                    *str_start++ = '\0';

                    const int num = atoi(cubes+1);

                    if (strcmp(str_start, "red") == 0) {
                        set->red = num;
                    } else if (strcmp(str_start, "green") == 0) {
                        set->green = num;
                    } else if (strcmp(str_start, "blue") == 0) {
                        set->blue = num;
                    }

                    cubes = strtok(NULL, ",");
                }

                // printf("the set we made is {r=%d,g=%d,b=%d}\n", set->red, set->green, set->blue);

                set_start = set_end+1;
                set_end = strchr(set_end+1, ';');
                if (set_end == NULL) {
                    set_end = sets_end;
                }
            }
        }

        // Part 1
        if (game_is_possible(game)) {
            part1sum += game->id;
        }
    }

    printf("Part 1 answer %d\n", part1sum);

    fclose(fp);
    if (line)
        free(line);
    return EXIT_SUCCESS;
}
