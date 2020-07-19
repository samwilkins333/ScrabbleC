#include "generator.h"

trie_node_t *trie_root;

int computeAllCandidates(list_t *rack, size_t dim, board_state_unit_t *played[dim][dim], list_t *all) {
    size_t existing_tile_count = 0;
    if (!validate_input(dim, played, rack, &existing_tile_count)) {
        return 0;
    }

    list_size(rack_size, rack, r, tile_t, link)
    if (rack_size) {
        if (!existing_tile_count) {
            size_t midpoint = dim / 2;
            generate_at_hook(midpoint, midpoint, rack, all, dim, played);
        } else {
            for (int y = 0; y < DIMENSIONS; y++) {
                for (int x = 0; x < DIMENSIONS; x++) {
                    for (int i = 0; i < 4; ++i) {
                        if (next_tile(x, y, all_directions[i], dim, played, NULL)) {
                            generate_at_hook(x, y, rack, all, dim, played);
                            break;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

void generate_at_hook(int x, int y, list_t *rack, list_t *all, size_t dim, board_state_unit_t *played[dim][dim]) {
    list_t placed;
    for (int i = 0; i < 2; ++i) {
        list_init(&placed);
        generate(x, y, x, y, rack, &placed, 0, all, trie_root, main_directions[i], dim, played);
    }
}

int validate_input(size_t dim, board_state_unit_t *played[dim][dim], list_t *rack, size_t *existing_tile_count) {
    if (!trie_root) {
        return 0;
    }
    if (dim < 3 || dim % 2 == 0) {
        return 0;
    }
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            if (!played[y][x]->multiplier) {
                return 0;
            }
            if (played[y][x]->tile) {
                (*existing_tile_count)++;
            }
        }
    }
    list_size(rack_size, rack, r, tile_t, link)
    if (rack_size > RACK_CAPACITY) {
        return 0;
    }
    return 1;
}

static inline void generate(size_t h_x, size_t h_y, size_t x, size_t y, list_t *rack, list_t *placed,
        int accumulated, list_t *all, trie_node_t *node, direction_t *d, size_t dim, board_state_unit_t *played[dim][dim]) {
    tile_t *tile = played[y][x]->tile;
    direction_t *i = inverse(d);
    trie_node_t *child_node;

    if (!tile) {
        list_size(current_placed_count, placed, p, enriched_tile_placement_t, link)
        list_size(rack_count, rack, r, tile_t, link)

        if (rack_count) {
            int visited[27];
            memset(&visited, 0, sizeof(visited));
            for (int r = 0; r < rack_count; ++r) {
                tile_t *to_place = list_head(rack, tile_t, link);
                list_remove(&to_place->link);
                char letter = to_place->letter;
                if (!visited[letter  - 97]) {
                    visited[letter - 97] = 1;
                    if (letter == BLANK) {
                        for (int l = 0; l < 26; ++l) {
                            try_letter_placement(h_x, h_x, x, y, rack, placed, accumulated, all, node, d, dim, played, to_place, alphabet[l], 1, current_placed_count);
                        }
                    } else {
                        try_letter_placement(h_x, h_x, x, y, rack, placed, accumulated, all, node, d, dim, played, to_place, letter, 0, current_placed_count);
                    }
                }
                list_insert_tail(rack, &to_place->link);
            }
        }

        trie_node_t *cross_anchor;
        coordinates_t next;
        if (current_placed_count && (cross_anchor = trie_node_get_child(node, DELIMITER)) && next_coordinates(h_x, h_y, i, &next)) {
            generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, cross_anchor, i, dim, played);
        }
    } else if (node && (child_node = trie_node_get_child(node, tile->resolved_letter()))) {
        evaluate_and_proceed(h_x, h_y, x, y, rack, placed, accumulated + tile->value, all, child_node, d, dim, played);
    }
}

static inline void try_letter_placement(size_t  h_x, size_t h_y, size_t x, size_t y, list_t *rack, list_t *placed,
                                        size_t accumulated, list_t *all, trie_node_t *node, direction_t *d, size_t dim,
                                        board_state_unit_t *played[dim][dim], tile_t *to_place, const char letter,
                                        int is_blank, size_t current_placed_count) {
    tile_t *resolved_tile = to_place;
    if (is_blank) {
        memset(resolved_tile = (tile_t *)malloc(sizeof(tile_t)), 0, sizeof(tile_t));
        memcpy(resolved_tile, to_place, sizeof(tile_t));
        resolved_tile->letter_proxy = letter;
    }
    trie_node_t *child;
    enriched_tile_placement_t *enriched = (enriched_tile_placement_t *)malloc(sizeof(enriched_tile_placement_t));
    memset(enriched, 0, sizeof(enriched_tile_placement_t));
    list_init(&enriched->cross);
    if ((child = trie_node_get_child(node, letter)) && compute_cross_word(x, y, to_place, d, dim, played, &enriched->cross)) {
        tile_placement_t *root = (tile_placement_t *)malloc(sizeof(tile_placement_t));
        memset(root, 0, sizeof(tile_placement_t));
        root->tile = resolved_tile;
        root->x = x;
        root->y = y;
        enriched->root = root;
        list_insert_tail(placed, &enriched->link);
        evaluate_and_proceed(h_x, h_y, x, y, rack, placed, accumulated, all, child, d, dim, played);
        while (1) {
            list_size(size, placed, p, enriched_tile_placement_t, link)
            if (size == current_placed_count) {
                break;
            }
            enriched_tile_placement_t *to_remove = list_tail(placed, enriched_tile_placement_t, link);
            list_remove(&to_remove->link);
            free(to_remove);
        }
    } else {
        free(enriched);
    }
}

static inline void evaluate_and_proceed(size_t  h_x, size_t h_y, size_t x, size_t y, list_t *rack, list_t *placed,
                                        size_t accumulated, list_t *all, trie_node_t *node, direction_t *d,
                                        size_t dim, board_state_unit_t *played[dim][dim]) {
    size_t total_score;
    direction_t *i = inverse(d);
    if (node->is_terminal && !next_tile(x, y, d, dim, played, NULL)) {
        if (d == &left || d == &up || !next_tile(h_x, h_y, i, dim, played, NULL)) {
            if ((total_score = apply_scorer(placed, dim, played, accumulated))) {
                scored_candidate_t *scored_candidate = (scored_candidate_t *)malloc(sizeof(scored_candidate_t));
                memset(scored_candidate, 0, sizeof(scored_candidate_t));
                scored_candidate->direction = normalize(d);
                scored_candidate->score = total_score;
                list_init(&scored_candidate->placements);
                list_iterate(placed, enriched, enriched_tile_placement_t, link) {
                    list_insert_tail(&scored_candidate->placements, &enriched->root->link);
                }
                list_insert_tail(all, &scored_candidate->link);
            }
        }
    }
    coordinates_t next;
    trie_node_t *cross_anchor;
    if (next_coordinates(x, y, d, &next)) {
        generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, node, d, dim, played);
    } else if ((cross_anchor = trie_node_get_child(node, DELIMITER)) && next_coordinates(h_x, h_y, i, &next)) {
        generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, cross_anchor, i, dim, played);
    }
}

static inline int compute_cross_word(size_t  s_x, size_t  s_y, tile_t *to_place, direction_t *d,
                                     size_t dim, board_state_unit_t *played[dim][dim], list_t *collector) {
    d = perpendicular(d);
    if (!next_tile(s_x, s_y, d, dim, played, NULL) || !next_tile(s_x, s_y, inverse(d), dim, played, NULL)) {
        return 1;
    }
    tile_t *tile = to_place;
    trie_node_t *node = trie_root;
    size_t x = s_x;
    size_t y = s_y;
    direction_t *original = d;
    list_t temporary;
    list_init(&temporary);

    while (tile) {
        tile_placement_t *placement = (tile_placement_t *)malloc(sizeof(tile_placement_t));
        memset(placement, 0, sizeof(tile_placement_t));
        placement->x = x;
        placement->y = y;
        placement->tile = tile;
        list_insert_tail(&temporary, &placement->link);
        if (!(node = trie_node_get_child(node, tile->resolved_letter()))) {
            break;
        }
        tile_placement_t next;
        if (next_tile(x, y, d, dim, played, &next)) {
            x = next.x;
            y = next.y;
            tile = next.tile;
        } else {
            d = inverse(d);
            if (d == original || !next_tile(s_x, s_y, d, dim, played, &next)) {
                break;
            }
            x = next.x;
            y = next.y;
            tile = next.tile;
            if (tile && !(node = trie_node_get_child(node, DELIMITER))) {
                break;
            }
        }
    }

    if (node && node->is_terminal) {
        list_iterate(&temporary, placement, tile_placement_t, link) {
            list_remove(&placement->link);
            list_insert_tail(collector, &placement->link);
        }
        return 1;
    }
    return 0;
}

static inline int apply_scorer(list_t *placed, size_t dim, board_state_unit_t *played[dim][dim], size_t  accumulated) {
    list_t primary;
    list_init(&primary);
    int sum = 0;
    list_iterate(placed, placement, enriched_tile_placement_t, link) {
        list_insert_tail(&primary, &placement->root->link);
        if (!list_empty(&placement->cross)) {
            sum += compute_score_of(dim, played, &placement->cross, 0);
        }
    }
    sum += compute_score_of(dim, played, &primary, accumulated);
    return sum;
}

static inline int compute_score_of(size_t dim, board_state_unit_t *played[dim][dim], list_t *placements, size_t sum) {
    int word_multiplier = 1;
    int new_tiles = 0;

    list_iterate(placements, placement, tile_placement_t, link) {
        tile_t *tile = placement->tile;
        board_state_unit_t *state = played[placement->y][placement->x];
        if (!state->tile) {
            new_tiles++;
        }
        multiplier_t *multiplier = state->multiplier;
        if (!multiplier || state->tile) {
            sum += tile->value;
        } else {
            sum += (multiplier->letter * tile->value);
            word_multiplier *= multiplier->word;
        }
    }

    size_t total = sum * word_multiplier;
    if (new_tiles == RACK_CAPACITY) {
        total += 50;
    }
    return total;
}