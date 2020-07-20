#pragma once

#include <stdlib.h>
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
    list_link_t cleanup_link;
} tile_t;

typedef struct tile_placement {
    int x;
    int y;
    tile_t *tile;
    list_link_t link;
    list_link_t cleanup_link;
} tile_placement_t;

typedef struct multiplier {
    int letter;
    int word;
} multiplier_t;

typedef struct board_state_unit {
    multiplier_t *multiplier;
    tile_t *tile;
} board_state_unit_t;

typedef struct enriched_tile_placement {
    tile_placement_t *root;
    list_t cross;
    list_link_t link;
} enriched_tile_placement_t;

#include "configuration.h"

typedef struct direction {
    const char *name;
    int x_inc;
    int y_inc;
} direction_t;

extern direction_t left;
extern direction_t right;
extern direction_t up;
extern direction_t down;

extern direction_t *main_directions[];
extern direction_t *all_directions[];

direction_t *inverse(direction_t *direction);
direction_t *perpendicular(direction_t *direction);
direction_t *normalize(direction_t *direction);
int next_coordinates(int x, int y, direction_t *d, coordinates_t *next);
int next_tile(int x, int y, direction_t *d, size_t dim, board_state_unit_t *played[dim][dim], tile_placement_t *out);

typedef struct scored_candidate {
    tile_placement_t **placements;
    size_t placements_count;
    int score;
    direction_t *direction;
    char *serialized;
    list_link_t link;
} scored_candidate_t;