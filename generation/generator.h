#pragma once

#include "configuration.h"
#include "../vocabulary/trie_node.h"
#include "../utilities/list.h"
#include "models.h"
#include <stdlib.h>

static char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{' };

generation_result_t *compute(list_t *rack, size_t dim, board_state_unit_t *played[dim][dim]);
int compare_candidates(const void *one, const void *two);
int validate_input(size_t dim, board_state_unit_t *played[dim][dim], list_t *rack, size_t *existing_tile_count);
void generate_at_hook(int x, int y, list_t *rack, list_t *all, size_t dim, board_state_unit_t *played[dim][dim], generation_result_t *result);
static void generate(size_t h_x, size_t  h_y, size_t  x, size_t  y, list_t *rack, list_t *placed,
                     int accumulated, list_t *all, trie_node_t *node, direction_t *d, size_t dim,
                     board_state_unit_t *played[dim][dim], generation_result_t *result);
static inline void evaluate_and_proceed(size_t  h_x, size_t  h_y, size_t  x, size_t  y, list_t *rack, list_t *placed,
                                        size_t  accumulated, list_t *all, trie_node_t *node, direction_t *d, size_t dim,
                                        board_state_unit_t *played[dim][dim], generation_result_t *result);
static inline void try_letter_placement(size_t  h_x, size_t  h_y, size_t  x, size_t  y, list_t *rack, list_t *placed,
                                        size_t  accumulated, list_t *all, trie_node_t *node, direction_t *d,
                                        size_t dim, board_state_unit_t *played[dim][dim], tile_t *to_place, char letter,
                                        int is_blank, size_t  current_placed_count, generation_result_t *result);
static inline int compute_cross_word(size_t  s_x, size_t  s_y, tile_t *to_place, direction_t *d, size_t dim,
        board_state_unit_t *played[dim][dim], list_t *cross, generation_result_t *result);
static inline int apply_scorer(list_t *placed, size_t dim, board_state_unit_t *played[dim][dim], size_t  accumulated);
static inline int compute_score_of(size_t dim, board_state_unit_t *played[dim][dim], list_t *placements, size_t  sum);
void generator_clean_up(generation_result_t *result);

static inline int compare_x(const void *one, const void *two) {
    return (*((tile_placement_t **)one))->x - (*((tile_placement_t **)two))->x;
}

static inline int compare_y(const void *one, const void *two) {
    return (*((tile_placement_t **)one))->y - (*((tile_placement_t **)two))->y;
}