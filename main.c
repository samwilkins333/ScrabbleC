#include "vocabulary/trie_factory.h"
#include "generation/generator.h"

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
    computeAllCandidates(&rack, (board_state_unit_t ***) played, DIMENSIONS, &all);
    return 0;
}