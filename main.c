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
    char letters[] = "{gofj{p";
    for (int i = 0; i < RACK_CAPACITY; i++) {
        tile_t *tile = (tile_t *)malloc(sizeof(tile_t));
        memset(tile, 0, sizeof(tile_t));
        tile->letter = letters[i];
        tile->value = 1;
        list_insert_tail(&rack, &tile->link);
    }
    size_t count;
    scored_candidate_t **candidates = compute_all_candidates(&rack, DIMENSIONS, played, &count);
    printf("Found %ld candidates.\n", count);
    char *output = malloc(7 * sizeof(char));
    for (int c = 0; c < count; c++) {
        scored_candidate_t *candidate = candidates[c];
        memset(output, 0, 7);
        for (int i = 0; i < candidate->placements_count; i++) {
            tile_t *tile = candidate->placements[i]->tile;
            sprintf(output, "%s%c", output, tile->letter_proxy ? tile->letter_proxy : tile->letter);
        }
        printf("%s [%d] %s\n", output, candidate->score, candidate->direction->name);
    }
    free(output);

    return 0;
}