#include "../utilities/list.h"

typedef struct coordinates {
    int x;
    int y;
} coordinates_t;

typedef struct tile {
    char letter;
    char letter_proxy;
    int value;
    list_link_t link;
} tile_t;

typedef struct tile_placement {
    int x;
    int y;
    tile_t tile;
    list_link_t link;
} tile_placement_t;

typedef struct multiplier {
    int letter;
    int word;
} multiplier_t;

typedef struct board_state_unit {
    multiplier_t *multiplier;
    tile_t *tile;
} board_state_unit_t;

typedef struct scored_candidate {
    list_t placements;
    int score;
    direction_t direction;
    list_link_t link;
} scored_candidate_t;