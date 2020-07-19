#include "configuration.h"
#include "../vocabulary/trie_node.h"
#include "../utilities/list.h"
#include "models.h"
#include <stdlib.h>

static trie_node_t *trie_root;

static direction_t *main_directions[] = { &right, &down };
static direction_t *all_directions[] = { &left, &right, &up, &down };

int computeAllCandidates(list_t *rack, board_state_unit_t ***played, size_t dimensions, list_t *all) ;
int validate_input(board_state_unit_t ***played, list_t *rack, size_t dimensions, size_t *existing_tile_count);
void generate_at_hook(int x, int y, list_t *rack, list_t *all, board_state_unit_t ***played);
static void generate(size_t h_x, size_t  h_y, size_t  x, size_t  y, list_t *rack, list_t *placed,
                     int accumulated, list_t *all, trie_node_t *node, direction_t *d, board_state_unit_t ***played);
static inline void evaluate_and_proceed(size_t  h_x, size_t  h_y, size_t  x, size_t  y, list_t *rack, list_t *placed,
                                        size_t  accumulated, list_t *all, trie_node_t *node, direction_t *d, board_state_unit_t ***played);
static inline void try_letter_placement(size_t  h_x, size_t  h_y, size_t  x, size_t  y, list_t *rack, list_t *placed,
                                        size_t  accumulated, list_t *all, trie_node_t *node, direction_t *d,
                                        board_state_unit_t ***played, tile_t *to_place, char letter,
                                        int is_blank, size_t  current_placed_count);
static inline int compute_cross_word(size_t  s_x, size_t  s_y, tile_t *to_place, direction_t *d, board_state_unit_t ***played, list_t *collector);
static inline int apply_scorer(list_t *placed, board_state_unit_t ***played, size_t  accumulated);
static inline int compute_score_of(board_state_unit_t ***played, list_t *placements, size_t  sum);