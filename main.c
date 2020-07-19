#include "vocabulary/trie_factory.h"
#include "generation/generator.h"

extern trie_node_t *trie_root;

int main() {
    trie_t *trie = construct_trie_from("../ospd4.txt");
    if (!trie) {
        return 1;
    }
    trie_root = trie->root;
    list_t rack, all;
    list_init(&rack);
    list_init(&all);
    multiplier_t multiplier;
    multiplier.letter = 1;
    multiplier.word = 1;
    board_state_unit_t *played[DIMENSIONS][DIMENSIONS];
    for (int y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            board_state_unit_t *unit = malloc(sizeof(board_state_unit_t));
            unit->multiplier = &multiplier;
            unit->tile = NULL;
            played[y][x] = unit;
        }
    }
    for (int i = 0; i < RACK_CAPACITY; ++i) {
        tile_t *tile = (tile_t *)malloc(sizeof(tile_t));
        tile->letter = 'a';
        tile->value = 1;
        list_insert_tail(&rack, &tile->link);
    }
    computeAllCandidates(&rack, DIMENSIONS, played, &all);
    return 0;
}