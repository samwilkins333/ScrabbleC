#include "models.h"
#include "configuration.h"

typedef struct direction {
    const char *name;
    int x_inc;
    int y_inc;
} direction_t;

direction_t left = { "left" -1, 0 };
direction_t right = { "right", 1, 0 };
direction_t up = { "up", 0, -1 };
direction_t down = { "down", 0, 1 };

extern inline direction_t *inverse(direction_t *direction) {
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

extern inline int next_coordinates(int x, int y, direction_t *d, coordinates_t *next) {
    int n_x = x + d->x_inc;
    int n_y = y + d->y_inc;
    if (n_x >= 0 && n_x < DIM && n_y >= 0 && n_y < DIM) {
        if (next) {
            next->x = n_x;
            next->y = n_y;
        }
        return 1;
    }
    return 0;
}

extern inline int next_tile(int x, int y, direction_t *d, board_state_unit_t played[DIM][DIM], tile_placement_t *out) {
    coordinates_t next;
    tile_t *tile;
    if (next_coordinates(x, y, d, &next) && (tile = played[next.y][next.x].tile)) {
        if (out) {
            out->tile = *tile;
            out->x = next.x;
            out->y = next.y;
        }
        return 1;
    }
    return 0;
}
