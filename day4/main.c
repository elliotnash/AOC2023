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
    int winners_len;
    int* winners;
    int numbers_len;
    int* numbers;
} card_t;

int count_cards(const card_t* cards, const int line) {
    // printf("calling count_cards for line %d\n", line);
    int matches = 0;
    for (int i = 0; i < cards[line].numbers_len; ++i) {
        for (int j = 0; j < cards[line].winners_len; ++j) {
            if (cards[line].winners[j] == cards[line].numbers[i]) {
                ++matches;
                break;
            }
        }
    }
    int new_matches = 0;
    for (int i = 1; i <= matches; ++i) {
        new_matches += count_cards(cards, line+i);
    }
    return 1 + new_matches;
}

int main() {
    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }


    int part1sum = 0;
    int part2sum = 0;

    card_t* cards = malloc(sizeof(card_t) * 250);

    int num_cards = 0;
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

        int* numbers = malloc(sizeof(int) * 30);
        num = strtok(pipe+1, " ");
        int numbers_len = 0;
        int points = 0;
        int match_val = 1;
        while (num) {
            const int val = atoi(num);
            numbers[numbers_len++] = val;
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

        cards[num_cards++] = (card_t) {num_winners, winners, numbers_len, numbers};
    }

    // Part 2
    for (int i = 0; i < num_cards; ++i) {
        part2sum += count_cards(cards, i);
    }

    printf("Part 1 answer %d\n", part1sum);
    printf("Part 2 answer %d\n", part2sum);

    fclose(fp);
    free(line);
    free(cards);
    return EXIT_SUCCESS;
}
