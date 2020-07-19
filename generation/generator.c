#include "configuration.h"
#include "../vocabulary/trie_node.h"
#include "../utilities/list.h"
#include "models.h"
#include <stdlib.h>

list_t computeAllCandidates(list_t *rack, board_state_unit_t played[DIM][DIM]) {
    list_t all;
    list_init(&all);

}

static void generate(int h_x, int h_y, int x, int y, list_t *rack, list_t *placed,
        int accumulated, list_t *all, trie_node_t *node, direction_t *d, board_state_unit_t played[DIM][DIM]) {
    tile_t *existingTile = played[y][x].tile;
    direction_t *i = inverse(d);
    trie_node_t child_node;

    if (!existingTile) {
        list_size(current_placed_count, placed, p, enriched_tile_placement_t, link);
        list_size(rack_count, rack, r, tile_t, link);

        if (rack_count) {
            for (int r = 0; r < rack_count; ++r) {
                tile_t *to_place = list_head(rack, tile_t, link);
                list_remove(&to_place->link);
                char letter = to_place->letter;

            }
        }
    }

}

static inline int apply_scorer(list_t *placed, board_state_unit_t played[DIM][DIM], int score) {

}

static inline void evaluate_and_proceed(int h_x, int h_y, int x, int y, list_t *rack, list_t *placed,
        int accumulated, list_t *all, trie_node_t *node, direction_t *d, board_state_unit_t played[DIM][DIM]) {
    int total_score;
    direction_t *i = inverse(d);
    if (node->is_terminal && !next_tile(x, y, d, played, NULL)) {
        if (d == &left || d == &up || !next_tile(h_x, h_y, i, played, NULL)) {
            if ((total_score = apply_scorer(placed, played, accumulated))) {
                scored_candidate_t *scored_candidate = (scored_candidate_t *)malloc(sizeof(scored_candidate_t));
                scored_candidate->direction = normalize(d);
                scored_candidate->score = total_score;
                list_init(&scored_candidate->placements);
                list_iterate(placed, placement, enriched_tile_placement_t, link) {
                    list_remove(&placement->link);
                    list_insert_tail(&scored_candidate->placements, &placement->link);
                }
                list_insert_tail(all, &scored_candidate->link);
            }
        }
        coordinates_t next;
        trie_node_t *cross_anchor;
        if (next_coordinates(x, y, d, &next)) {
            generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, node, d, played);
        } else if ((cross_anchor = trie_node_get_child(node, DELIMITER)) && next_coordinates(h_x, h_y, i, &next)) {
            generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, cross_anchor, i, played);
        }
    }
}


