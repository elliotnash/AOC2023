#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

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

typedef struct {
    int line;
    int start;
    int end;
    int value;
} part_t;

part_t part_from_match(char** lines, int i, int j) {
    // Find end
    int end = j;
    while (isnumber(lines[i][++end])) {}
    --end;
    // Find start
    int start = j;
    while (isnumber(lines[i][--start])) {}
    ++start;
    // Get value
    const int value = atoin(lines[i]+start, lines[i]+end);
    return (part_t) {i, start, end, value};
}

bool in_part(const part_t* part, const int y, const int x) {
    return y == part->line && x >= part->start && x <= part->end;
}

bool in_parts(const part_t* parts, const int length, const int y, const int x) {
    for (int i = 0; i < length; ++i) {
        if (in_part(parts+i, y, x)) {
            return true;
        }
    }
    return false;
}

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }

    int part1sum = 0;
    int part2sum = 0;

    char** lines = malloc(sizeof(char*) * 100);
    size_t num_lines = 0;

    size_t len = 0;
    ssize_t line_len;
    while ((line_len = getline(lines + num_lines, &len, fp)) != -1) {
        if (lines[num_lines][line_len-1] == '\n') {
            lines[num_lines][line_len-1] = '\0';
        }
        ++num_lines;
    }

    // Part 1
    {
        for (int i = 0; i < num_lines; ++i) {
            for (int j = 0; lines[i][j] != '\0';) {
                if (isnumber(lines[i][j])) {
                    int start = j;
                    bool is_part = false;
                    // Check character before number
                    if (j>0) {
                        if (lines[i][j-1] != '.') {
                            is_part = true;
                        }
                        if (i > 0 && lines[i-1][j-1] != '.') {
                            is_part = true;
                        }
                        if (i+1 < num_lines && lines[i+1][j-1] != '.') {
                            is_part = true;
                        }
                    }
                    // Check characters above and below number
                    while (isnumber(lines[i][j])) {
                        if (i > 0 && lines[i-1][j] != '.') {
                            is_part = true;
                        }
                        if (i+1 < num_lines && lines[i+1][j] != '.') {
                            is_part = true;
                        }
                        ++j;
                    }
                    // We hit end of number, mark it
                    const int end = j-1;
                    // Check character after number
                    if (lines[i][j] != '\0') {
                        if (lines[i][j] != '.') {
                            is_part = true;
                        }
                        if (i > 0 && lines[i-1][j] != '.') {
                            is_part = true;
                        }
                        if (i+1 < num_lines && lines[i+1][j] != '.') {
                            is_part = true;
                        }
                    }

                    // If it is a part, we need to parse the number and sum it
                    if (is_part) {
                        part1sum += atoin(lines[i]+start, lines[i]+end);
                    }
                } else {
                    ++j;
                }
            }
        }
    }

    // Part 2
    {
        for (int i = 0; i < num_lines; ++i) {
            for (int j = 0; lines[i][j] != '\0'; ++j) {
                if (lines[i][j] == '*') {
                    // We've found a potential gear ratio to search
                    part_t* parts = malloc(sizeof(part_t) * 8);
                    int num_parts = 0;
                    // Loop through all adjecent positions.
                    for (int y = i-1; y <= i+1; ++y) {
                        for (int x = j-1; x <= j+1; ++x) {
                            // Make sure position is valid
                            if (y >= 0 && x >= 0 && y < num_lines && isnumber(lines[y][x])) {
                                // If this is a new adjecent number, add it
                                if (!in_parts(parts, num_parts, y, x)) {
                                    parts[num_parts++] = part_from_match(lines, y, x);
                                }
                            } else {
                            }
                        }
                    }

                    if (num_parts == 2) {
                        // If we have exactly 2 adjecent parts, add the gear ratio
                        part2sum += parts[0].value * parts[1].value;
                    }

                    free(parts);
                }
            }
        }
    }

    printf("Part 1 answer %d\n", part1sum);
    printf("Part 2 answer %d\n", part2sum);

    fclose(fp);
    free(lines);
    return EXIT_SUCCESS;
}
