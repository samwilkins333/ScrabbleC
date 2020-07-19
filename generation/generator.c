#include "configuration.h"
#include "../vocabulary/trie_node.h"
#include "direction.h"
#include <stdlib.h>

list_t computeAllCandidates(list_t *rack, board_state_unit_t played[DIM][DIM]) {
    list_t all;
    list_init(&all);

}

static inline void generate_at_hook(int x, int y, list_t *all) {

}

static void generate(int h_x, int h_y, int x, int y, list_t *rack, list_t *placed,
        int accumulated, list_t *all, trie_node_t node, direction_t d, board_state_unit_t played[DIM][DIM]) {
    tile_t *existing = played[y][x].tile;
    direction_t *i = inverse(&d);
    trie_node_t child_node;



}

static inline int apply_scorer(list_t placed, board_state_unit_t played[DIM][DIM], int score) {

}

static inline void evaluate_and_proceed(int h_x, int h_y, int x, int y, direction_t *d, board_state_unit_t played[DIM][DIM], trie_node_t child_node) {
    int total_score;
    if (child_node.is_terminal && !next_tile(x, y, d, played, NULL)) {
        if (d == &left || d == &up || !next_tile(h_x, h_y, inverse(d), played, NULL)) {
            if ((total_score = apply_scorer(placed, played, )))
        }
    }
}


