typedef struct coordinates {
    int x;
    int y;
} coordinates_t;

typedef struct tile {
    char letter;
    char letter_proxy;
    int value;
} tile_t;

typedef struct tile_placement {
    coordinates_t coordinates;
    tile_t tile;
} tile_placement_t;

typedef struct multiplier {
    int letter;
    int word;
} multiplier_t;

typedef struct board_state_unit {
    multiplier_t multiplier;
    tile_t tile;
} board_state_unit_t;