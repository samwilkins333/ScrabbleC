#include "vocabulary/trie_factory.h"
#include "generation/generator.h"
#include <time.h>
#include <sys/time.h>

#define LOGGING

extern trie_node_t *trie_root;
int allocations = 0;

int main() {
    // initialize trie
    trie_t *trie = trie_factory(DICTIONARY);
    if (!trie) {
        return 1;
    }
    trie_root = trie->root;

    // initialize rack
    list_t rack;
    list_init(&rack);
    char letters[] = "au{sero";
    for (int i = 0; i < RACK_CAPACITY; i++) {
        NEW(tile_t, tile, 1);
        tile->letter = letters[i];
        tile->value = 1;
        list_insert_tail(&rack, &tile->link);
    }

    // initialize board
    multiplier_t multiplier;
    multiplier.letter = 1;
    multiplier.word = 1;
    board_state_unit_t *played[DIMENSIONS][DIMENSIONS];
    for (int y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            NEW(board_state_unit_t, unit, 1);
            unit->multiplier = &multiplier;
            unit->tile = NULL;
            played[y][x] = unit;
        }
    }

    long start, mark;
    struct timeval time_check;

    gettimeofday(&time_check, NULL);
    start = (long) time_check.tv_sec * 1000 + (long) time_check.tv_usec / 1000;

    // generate candidates
    generation_result_t *result = compute(&rack, DIMENSIONS, played);

    gettimeofday(&time_check, NULL);
    mark = (long) time_check.tv_sec * 1000 + (long) time_check.tv_usec / 1000;

#ifdef LOGGING
    NEW(char, word_display, 13);
    NEW(char, location, 35);
    printf("%-13s  score%-3s %-11s <location>\n\n", "word", "", "direction");
    size_t invalid = 0;
    for (size_t c = 0; c < result->count; c++) {
        candidate_t *candidate = result->candidates[c];
        memset(word_display, 0, 13);
        memset(location, 0, 35);
        for (size_t i = 0; i < candidate->placements_count; i++) {
            tile_placement_t *placement = candidate->placements[i];
            tile_t *tile = placement->tile;
            if (tile->letter_proxy) {
                sprintf(word_display, "%s(%c*)", word_display, tile->letter_proxy);
            } else {
                sprintf(word_display, "%s%c", word_display, tile->letter);
            }
            sprintf(location, "%s(%d,%d)", location, placement->x, placement->y);
        }
        if (!trie_includes_word(trie, candidate->serialized)) {
            invalid++;
        }
        printf("%-13s  %-7d  %-10s  %s\n", word_display, candidate->score, candidate->direction->name, location);
    }

    DONE(word_display);
    DONE(location);

    printf("\nFound %ld candidates in %ld ms.\n", result->count, mark - start);

    if (invalid) {
        printf("%ld words were invalid.\n", invalid);
    } else {
        printf("All words are valid.\n");
    }
#endif

    // clean up candidates
    generator_clean_up(result);

    // clean up board
    for (int y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            DONE(played[y][x]);
        }
    }

    // clean up rack
    list_iterate(&rack.anchor, tile, tile_t, link) {
        DONE(tile);
    }

    // clean up trie
    trie_destroy(trie);

    if (allocations) {
        printf("\nMemory leak: %d allocations remaining.\n", allocations);
        return 1;
    }

    return 0;
}