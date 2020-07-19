#include "configuration.h"
#include "../vocabulary/trie_node.h"
#include "../utilities/list.h"
#include "models.h"
#include <stdlib.h>

list_t computeAllCandidates(list_t *rack, board_state_unit_t played[DIM][DIM]);
static void generate(int h_x, int h_y, int x, int y, list_t *rack, list_t *placed,
                     int accumulated, list_t *all, trie_node_t *node, direction_t *d, board_state_unit_t played[DIM][DIM]);
static inline void evaluate_and_proceed(int h_x, int h_y, int x, int y, list_t *rack, list_t *placed,
                                        int accumulated, list_t *all, trie_node_t *node, direction_t *d, board_state_unit_t played[DIM][DIM]);
static inline int apply_scorer(list_t *placed, board_state_unit_t played[DIM][DIM], int accumulated);
static inline int compute_score_of(board_state_unit_t played[DIM][DIM], list_t *placements, int sum);