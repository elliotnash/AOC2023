#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "aoclib.h"

typedef enum {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
} hand_type_t;

char* hand_type_name(const hand_type_t type) {
    switch (type) {
        case HIGH_CARD:
            return "High card";
        case ONE_PAIR:
            return "One pair";
        case TWO_PAIR:
            return "Two pair";
        case THREE_OF_A_KIND:
            return "Three of a kind";
        case FULL_HOUSE:
            return "Full house";
        case FOUR_OF_A_KIND:
            return "Four of a kind";
        case FIVE_OF_A_KIND:
            return "Five of a kind";
    }
    return NULL;
}

typedef struct {
    char* hand;
    hand_type_t type;
    uint64_t bid;
} hand_t;

hand_type_t hand_type_p1(char* hand) {
    hash_map_t* map = hash_map_create(hash_char, 100, 0.5);

    for (char* c = hand; *c != '\0'; ++c) {
        uint64_t* val = hash_map_get(map, c);
        if (val == NULL) {
            val = malloc(sizeof(uint64_t));
            *val = 1;
            hash_map_put(map, c, val);
        } else {
            ++*val;
        }
    }

    hand_type_t type = HIGH_CARD;

    arraylist_t* entries = hash_map_entry_list(map);

    int pairs = 0;
    int triples = 0;
    for (int i = 0; i < entries->count; ++i) {
        const hash_entry_t* entry = entries->data[i];
        const uint64_t value = *(uint64_t*)entry->value;
        if (value == 2) {
            ++pairs;
        } else if (value == 3) {
            ++triples;
        } else if (value == 4) {
            type = FOUR_OF_A_KIND;
            break;
        } else if (value == 5) {
            type = FIVE_OF_A_KIND;
            break;
        }
    }

    if (pairs == 1) {
        if (triples == 1) {
            type = FULL_HOUSE;
        } else {
            type = ONE_PAIR;
        }
    } else if (pairs == 2) {
        type = TWO_PAIR;
    } else if (triples == 1) {
        type = THREE_OF_A_KIND;
    }

    // TODO we need to free shit
    hash_map_free(map);

    return type;
}

hand_type_t hand_type_p2(char* hand) {
    hash_map_t* map = hash_map_create(hash_char, 100, 0.5);

    for (char* c = hand; *c != '\0'; ++c) {
        uint64_t* val = hash_map_get(map, c);
        if (val == NULL) {
            val = malloc(sizeof(uint64_t));
            *val = 1;
            hash_map_put(map, c, val);
        } else {
            ++*val;
        }
    }

    char* joker_key = malloc(sizeof(char));
    *joker_key = 'J';
    const uint64_t* jokers_ptr = (uint64_t*)hash_map_get(map, joker_key);
    free(joker_key);

    const int jokers = jokers_ptr == NULL ? 0 : *jokers_ptr;

    hand_type_t type = HIGH_CARD;
    arraylist_t* entries = hash_map_entry_list(map);

    if (jokers == 0) {
        int pairs = 0;
        int triples = 0;
        for (int i = 0; i < entries->count; ++i) {
            const hash_entry_t* entry = entries->data[i];
            if (*(char*)entry->key != 'J') {
                const uint64_t value = *(uint64_t*)entry->value;
                if (value == 5) {
                    type = FIVE_OF_A_KIND;
                    break;
                }
                if (value == 4) {
                    type = FOUR_OF_A_KIND;
                    break;
                }
                if (value == 3) {
                    ++triples;
                } else if (value == 2) {
                    ++pairs;
                }
            }
        }

        if (type < FOUR_OF_A_KIND) {
            if (pairs == 1) {
                if (triples == 1) {
                    type = FULL_HOUSE;
                } else {
                    type = ONE_PAIR;
                }
            } else if (pairs == 2) {
                type = TWO_PAIR;
            } else if (triples == 1) {
                type = THREE_OF_A_KIND;
            }
        }
    } else if (jokers == 1) {
        int pairs = 0;
        for (int i = 0; i < entries->count; ++i) {
            const hash_entry_t* entry = entries->data[i];
            if (*(char*)entry->key != 'J') {
                const uint64_t value = *(uint64_t*)entry->value;
                if (value == 4) {
                    type = FIVE_OF_A_KIND;
                    break;
                }
                if (value == 3) {
                    type = FOUR_OF_A_KIND;
                    break;
                }
                if (value == 2) {
                    type = THREE_OF_A_KIND;
                    ++pairs;
                }
                if (type < THREE_OF_A_KIND &&   value == 1) {
                    type = ONE_PAIR;
                }
            }
        }
        if (pairs == 2) {
            type = FULL_HOUSE;
        }
    } else if (jokers == 2) {
        for (int i = 0; i < entries->count; ++i) {
            const hash_entry_t* entry = entries->data[i];
            if (*(char*)entry->key != 'J') {
                const uint64_t value = *(uint64_t*)entry->value;
                if (value == 3) {
                    type = FIVE_OF_A_KIND;
                    break;
                }
                if (value == 2) {
                    type = FOUR_OF_A_KIND;
                    break;
                }
                type = THREE_OF_A_KIND;
            }
        }
    } else if (jokers == 3) {
        for (int i = 0; i < entries->count; ++i) {
            const hash_entry_t* entry = entries->data[i];
            if (*(char*)entry->key != 'J') {
                const uint64_t value = *(uint64_t*)entry->value;
                if (value == 2) {
                    type = FIVE_OF_A_KIND;
                } else {
                    type = FOUR_OF_A_KIND;
                }
                break;
            }
        }
    } else {
        type = FIVE_OF_A_KIND;
    }

    // TODO we need to free shit
    hash_map_free(map);

    return type;
}

hash_map_t* p1_card_map;
hash_map_t* p2_card_map;

void card_map_put(hash_map_t* map, const char key, const int val) {
    char* k = malloc(sizeof(char));
    *k = key;
    int* v = malloc(sizeof(int));
    *v = val;
    hash_map_put(map, k, v);
}

int card_map_get(hash_map_t* map, const char key) {
    char* k = malloc(sizeof(char));
    *k = key;
    const int* value = hash_map_get(map, k);
    free(k);
    return *value;
}

void init_card_map() {
    p1_card_map = hash_map_create(hash_char, 32, 0.5);
    card_map_put(p1_card_map, '2', 2);
    card_map_put(p1_card_map, '3', 3);
    card_map_put(p1_card_map, '4', 4);
    card_map_put(p1_card_map, '5', 5);
    card_map_put(p1_card_map, '6', 6);
    card_map_put(p1_card_map, '7', 7);
    card_map_put(p1_card_map, '8', 8);
    card_map_put(p1_card_map, '9', 9);
    card_map_put(p1_card_map, 'T', 10);
    card_map_put(p1_card_map, 'J', 11);
    card_map_put(p1_card_map, 'Q', 12);
    card_map_put(p1_card_map, 'K', 13);
    card_map_put(p1_card_map, 'A', 14);

    p2_card_map = hash_map_create(hash_char, 32, 0.5);
    card_map_put(p2_card_map, 'J', 1);
    card_map_put(p2_card_map, '2', 2);
    card_map_put(p2_card_map, '3', 3);
    card_map_put(p2_card_map, '4', 4);
    card_map_put(p2_card_map, '5', 5);
    card_map_put(p2_card_map, '6', 6);
    card_map_put(p2_card_map, '7', 7);
    card_map_put(p2_card_map, '8', 8);
    card_map_put(p2_card_map, '9', 9);
    card_map_put(p2_card_map, 'T', 10);
    card_map_put(p2_card_map, 'Q', 12);
    card_map_put(p2_card_map, 'K', 13);
    card_map_put(p2_card_map, 'A', 14);
}

int p1_compare_hand(const hand_t* lhs, const hand_t* rhs) {
    if (lhs->type > rhs->type) {
        return -1;
    }
    if (lhs->type < rhs->type) {
        return 1;
    }
    const char* lh = lhs->hand;
    const char* rh = rhs->hand;
    while (*lh != '\0' && *rh != '\0') {
        const char lc = card_map_get(p1_card_map, *lh);
        const char rc = card_map_get(p1_card_map, *rh);
        const int diff = lc - rc;
        if (diff != 0) {
            return -diff;
        }
        ++lh;
        ++rh;
    }
    return 0;
}

int p2_compare_hand(const hand_t* lhs, const hand_t* rhs) {
    if (lhs->type > rhs->type) {
        return -1;
    }
    if (lhs->type < rhs->type) {
        return 1;
    }
    const char* lh = lhs->hand;
    const char* rh = rhs->hand;
    while (*lh != '\0' && *rh != '\0') {
        const char lc = card_map_get(p2_card_map, *lh);
        const char rc = card_map_get(p2_card_map, *rh);
        const int diff = lc - rc;
        if (diff != 0) {
            return -diff;
        }
        ++lh;
        ++rh;
    }
    return 0;
}

void print_hand(const hand_t* hand) {
    printf("{hand: \"%s\", type: %s, bid: %llu}", hand->hand, hand_type_name(hand->type), hand->bid);
}

int main() {
    init_card_map();

    FILE* fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Failed to open input.txt! (did you set the working directory?)\n");
        return EXIT_FAILURE;
    }

    heap_t* p1heap = heap_create(p1_compare_hand, 1000);
    heap_t* p2heap = heap_create(p2_compare_hand, 1000);

    uint64_t part1ans = 0;
    uint64_t part2ans = 0;

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        const char* space = strchr(line, ' ');
        char* hand = malloc(sizeof(char) * (space - line));
        strncpy(hand, line, space-line);

        const char* end = space;
        while(*++end != '\n' && *end != '\0') {}
        --end;

        const uint64_t bid = atouln(space+1, end);

        hand_t* hand_p1 = malloc(sizeof(hand_t));
        *hand_p1 = (hand_t) {hand, hand_type_p1(hand), bid};
        heap_insert(p1heap, hand_p1);

        hand_t* hand_p2 = malloc(sizeof(hand_t));
        *hand_p2 = (hand_t) {hand, hand_type_p2(hand), bid};
        heap_insert(p2heap, hand_p2);
    }

    // Part 1
    for (int i = 1; p1heap->count > 0; ++i) {
        const hand_t* hand = heap_remove(p1heap);
        part1ans += hand->bid * i;
    }

    // Part 2
    for (int i = 1; p2heap->count > 0; ++i) {
        const hand_t* hand = heap_remove(p2heap);
        part2ans += hand->bid * i;
    }

    printf("Part 1 answer %llu\n", part1ans);
    printf("Part 2 answer %llu\n", part2ans);

    fclose(fp);
    free(line);

    // TODO
    heap_free(p1heap);

    return EXIT_SUCCESS;
}
