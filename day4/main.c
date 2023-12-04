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

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }


    int part1sum = 0;
    int part2sum = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        // Part 1
        char* colon = strchr(line, ':');
        char* pipe = strchr(line, '|');
        *pipe = '\0';
        const char* num = strtok(colon+1, " ");

        int* winners = malloc(sizeof(int) * 10);
        int num_winners = 0;
        while (num) {
            winners[num_winners++] = atoi(num);
            num = strtok(NULL, " ");
        }

        num = strtok(pipe+1, " ");
        int points = 0;
        int match_val = 1;
        while (num) {
            int val = atoi(num);
            for (int i = 0; i < num_winners; ++i) {
                if (winners[i] == val) {
                    points = match_val;
                    match_val *= 2;
                    break;
                }
            }
            num = strtok(NULL, " ");
        }
        part1sum += points;

        printf("%d\n", num_winners);
        free(winners);
    }

    printf("Part 1 answer %d\n", part1sum);
    printf("Part 2 answer %d\n", part2sum);

    fclose(fp);
    free(line);
    return EXIT_SUCCESS;
}
