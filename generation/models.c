#include "models.h"

direction_t left = { "left" -1, 0 };
direction_t right = { "right", 1, 0 };
direction_t up = { "up", 0, -1 };
direction_t down = { "down", 0, 1 };

direction_t *main_directions[] = { &right, &down };
direction_t *all_directions[] = { &left, &right, &up, &down };

inline direction_t *inverse(direction_t *direction) {
    if (direction == &left) {
        return &right;
    }
    if (direction == &right) {
        return &left;
    }
    if (direction == &up) {
        return &down;
    }
    return &up;
}

inline direction_t *perpendicular(direction_t *direction) {
    if (direction == &left || direction == &right) {
        return &down;
    }
    return &right;
}

inline direction_t *normalize(direction_t *direction) {
    if (direction == &left || direction == &right) {
        return &right;
    }
    return &down;
}

inline int next_coordinates(int x, int y, direction_t *d, coordinates_t *next) {
    int n_x = x + d->x_inc;
    int n_y = y + d->y_inc;
    if (n_x >= 0 && n_x < DIMENSIONS && n_y >= 0 && n_y < DIMENSIONS) {
        if (next) {
            next->x = n_x;
            next->y = n_y;
        }
        return 1;
    }
    return 0;
}

inline int next_tile(int x, int y, direction_t *d, size_t dim,  board_state_unit_t *played[dim][dim], tile_placement_t *out) {
    coordinates_t next;
    tile_t *tile;
    if (next_coordinates(x, y, d, &next) && (tile = played[next.y][next.x]->tile)) {
        if (out) {
            out->tile = tile;
            out->x = next.x;
            out->y = next.y;
        }
        return 1;
    }
    return 0;
}