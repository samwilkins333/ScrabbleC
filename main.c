#include "vocabulary/trie_factory.h"
#include "generation/generator.h"

extern trie_node_t *trie_root;

int main() {
    trie_t *trie = construct_trie_from("../ospd4.txt");
    if (!trie) {
        return 1;
    }
    trie_root = trie->root;
    list_t rack;
    list_init(&rack);
    multiplier_t multiplier;
    multiplier.letter = 1;
    multiplier.word = 1;
    board_state_unit_t *played[DIMENSIONS][DIMENSIONS];
    for (int y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            board_state_unit_t *unit = malloc(sizeof(board_state_unit_t));
            memset(unit, 0, sizeof(board_state_unit_t));
            unit->multiplier = &multiplier;
            unit->tile = NULL;
            played[y][x] = unit;
        }
    }
    char letters[] = "au{sero";
    for (int i = 0; i < RACK_CAPACITY; i++) {
        tile_t *tile = (tile_t *)malloc(sizeof(tile_t));
        memset(tile, 0, sizeof(tile_t));
        tile->letter = letters[i];
        tile->value = 1;
        list_insert_tail(&rack, &tile->link);
    }
    size_t count;
    scored_candidate_t **candidates = compute_all_candidates(&rack, DIMENSIONS, played, &count);

    char *word_display = malloc(13 * sizeof(char));
    char *location = malloc(35 * sizeof(char));
    printf("%-13s  score%-3s %-11s <location>\n\n", "word", "", "direction");
    size_t invalid = 0;
    for (int c = 0; c < count; c++) {
        scored_candidate_t *candidate = candidates[c];
        memset(word_display, 0, 13);
        memset(location, 0, 35);
        for (int i = 0; i < candidate->placements_count; i++) {
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
    free(word_display);
    free(location);

    printf("\nFound %ld candidates.\n", count);
    if (invalid) {
        printf("%ld words were invalid.\n", invalid);
    } else {
        printf("All words are valid.\n");
    }

    return 0;
}