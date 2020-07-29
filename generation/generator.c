#include "generator.h"

trie_node_t *trie_root;
extern int allocations;

generation_result_t *compute(list_t *rack, size_t dim, board_state_unit_t *played[dim][dim]) {
    size_t existing_tile_count = 0;
    if (!validate_input(dim, played, rack, &existing_tile_count)) {
        return NULL;
    }

    NEW(generation_result_t, result, 1);
    list_init(&result->allocated_tile_placements);
    list_init(&result->allocated_tiles);

    list_t all;
    list_init(&all);
    if (rack->size) {
        if (!existing_tile_count) {
            size_t midpoint = dim / 2;
            generate_at_hook(midpoint, midpoint, rack, &all, dim, played, result);
        } else {
            for (int y = 0; y < DIMENSIONS; y++) {
                for (int x = 0; x < DIMENSIONS; x++) {
                    for (int i = 0; i < 4; ++i) {
                        if (next_tile(x, y, all_directions[i], dim, played, NULL)) {
                            generate_at_hook(x, y, rack, &all, dim, played, result);
                            break;
                        }
                    }
                }
            }
        }
    }

    NEW(candidate_t *, candidates, all.size);
    result->candidates = candidates;
    size_t i = 0;
    list_iterate(&all.anchor, candidate, candidate_t, link) {
        result->candidates[i++] = candidate;
    }

    qsort(result->candidates, all.size, sizeof(candidate_t *), compare_candidates);
    result->count = all.size;

    return result;
}

int compare_candidates(const void *one, const void *two) {
    candidate_t *c_one = *(candidate_t **)one;
    candidate_t *c_two = *(candidate_t **)two;
    int score_diff = c_two->score - c_one->score;
    if (score_diff) {
        return score_diff;
    }
    int alpha_diff = strcmp(c_one->serialized, c_two->serialized);
    if (alpha_diff) {
        return alpha_diff;
    }
    return strcmp(c_two->direction->name, c_one->direction->name);
}

void generate_at_hook(int x, int y, list_t *rack, list_t *all, size_t dim, board_state_unit_t *played[dim][dim], generation_result_t *result) {
    list_t placed;
    for (int i = 0; i < 2; ++i) {
        list_init(&placed);
        generate(x, y, x, y, rack, &placed, 0, all, trie_root, main_directions[i], dim, played, result);
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
    if (rack->size > RACK_CAPACITY) {
        return 0;
    }
    return 1;
}

static inline void generate(size_t h_x, size_t h_y, size_t x, size_t y, list_t *rack, list_t *placed,
        int accumulated, list_t *all, trie_node_t *node, direction_t *d, size_t dim,
        board_state_unit_t *played[dim][dim], generation_result_t *result) {
    tile_t *tile = played[y][x]->tile;
    direction_t *i = inverse(d);
    trie_node_t *child_node;

    if (!tile) {
        size_t current_placed_count = placed->size;
        size_t rack_count = rack->size;
        if (rack_count) {
            int visited[27];
            memset(&visited, 0, sizeof(visited));
            for (int r = 0; r < rack_count; ++r) {
                tile_t *to_place = list_head(&rack->anchor, tile_t, link);
                list_remove(rack, &to_place->link);
                char letter = to_place->letter;
                if (!visited[letter - 97]) {
                    visited[letter - 97] = 1;
                    if (letter == BLANK) {
                        for (int l = 0; l < 26; ++l) {
                            try_letter_placement(h_x, h_y, x, y, rack, placed, accumulated, all, node, d, dim, played, to_place, alphabet[l], 1, current_placed_count, result);
                        }
                    } else {
                        try_letter_placement(h_x, h_y, x, y, rack, placed, accumulated, all, node, d, dim, played, to_place, letter, 0, current_placed_count, result);
                    }
                }
                list_insert_tail(rack, &to_place->link);
            }
        }

        trie_node_t *cross_anchor;
        coordinates_t next;
        if (current_placed_count && (cross_anchor = trie_node_get_child(node, DELIMITER)) && next_coordinates(h_x, h_y, i, &next)) {
            generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, cross_anchor, i, dim, played, result);
        }
    } else if ((child_node = trie_node_get_child(node, resolved_letter(tile)))) {
        evaluate_and_proceed(h_x, h_y, x, y, rack, placed, accumulated + tile->value, all, child_node, d, dim, played, result);
    }
}

static inline void try_letter_placement(size_t  h_x, size_t h_y, size_t x, size_t y, list_t *rack, list_t *placed,
                                        size_t accumulated, list_t *all, trie_node_t *node, direction_t *d, size_t dim,
                                        board_state_unit_t *played[dim][dim], tile_t *to_place, const char letter,
                                        int is_blank, size_t current_placed_count, generation_result_t *result) {
    tile_t *resolved_tile = to_place;
    if (is_blank) {
        NEW(tile_t, proxied, 1);
        resolved_tile = proxied;
        list_insert_tail(&result->allocated_tiles, &resolved_tile->result_link);
        resolved_tile->letter = to_place->letter;
        resolved_tile->value = to_place->value;
        resolved_tile->letter_proxy = letter;
    }
    trie_node_t *child;
    NEW(enriched_tile_placement_t, enriched, 1);
    list_init(&enriched->cross);
    if ((child = trie_node_get_child(node, letter)) && compute_cross_word(x, y, resolved_tile, d, dim, played, &enriched->cross, result)) {
        NEW(tile_placement_t, root, 1);
        list_insert_tail(&result->allocated_tile_placements, &root->result_link);
        root->tile = resolved_tile;
        root->x = x;
        root->y = y;
        enriched->root = root;
        list_insert_tail(placed, &enriched->link);
        evaluate_and_proceed(h_x, h_y, x, y, rack, placed, accumulated, all, child, d, dim, played, result);
        while (1) {
            if (placed->size == current_placed_count) {
                break;
            }
            enriched_tile_placement_t *to_remove = list_tail(&placed->anchor, enriched_tile_placement_t, link);
            list_remove(placed, &to_remove->link);
            DONE(to_remove);
        }
    } else {
        DONE(enriched);
    }
}

static inline void evaluate_and_proceed(size_t  h_x, size_t h_y, size_t x, size_t y, list_t *rack, list_t *placed,
                                        size_t accumulated, list_t *all, trie_node_t *node, direction_t *d,
                                        size_t dim, board_state_unit_t *played[dim][dim], generation_result_t *result) {
    size_t total_score;
    direction_t *i = inverse(d);
    if (node->is_terminal && !next_tile(x, y, d, dim, played, NULL)) {
        if (d == &left || d == &up || !next_tile(h_x, h_y, i, dim, played, NULL)) {
            if ((total_score = apply_scorer(placed, dim, played, accumulated))) {
                NEW(candidate_t, candidate, 1);
                candidate->direction = normalize(d);
                candidate->score = total_score;
                NEW(tile_placement_t *, p, placed->size)
                candidate->placements = p;
                candidate->placements_count = placed->size;
                size_t j = 0;
                list_iterate(&placed->anchor, enriched, enriched_tile_placement_t, link) {
                    candidate->placements[j++] = enriched->root;
                }
                int (*compare)(const void *, const void *) = candidate->direction == &right ? compare_x : compare_y;
                qsort(candidate->placements, placed->size, sizeof(tile_placement_t *), compare);
                NEW(char, serialized, (placed->size + 1));
                candidate->serialized = serialized;
                for (int k = 0; k < placed->size; ++k) {
                    tile_t *tile =  candidate->placements[k]->tile;
                    sprintf(candidate->serialized, "%s%c", candidate->serialized, resolved_letter(tile));
                }
                list_insert_tail(all, &candidate->link);
            }
        }
    }
    coordinates_t next;
    trie_node_t *cross_anchor;
    if (next_coordinates(x, y, d, &next)) {
        generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, node, d, dim, played, result);
    } else if ((cross_anchor = trie_node_get_child(node, DELIMITER)) && next_coordinates(h_x, h_y, i, &next)) {
        generate(h_x, h_y, next.x, next.y, rack, placed, accumulated, all, cross_anchor, i, dim, played, result );
    }
}

static inline int compute_cross_word(size_t  s_x, size_t  s_y, tile_t *to_place, direction_t *d,
                                     size_t dim, board_state_unit_t *played[dim][dim], list_t *cross, generation_result_t *result) {
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
        NEW(tile_placement_t, placement, 1);
        list_insert_tail(&result->allocated_tile_placements, &placement->result_link);
        placement->x = x;
        placement->y = y;
        placement->tile = tile;
        list_insert_tail(&temporary, &placement->link);
        if (!(node = trie_node_get_child(node, resolved_letter(tile)))) {
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
        list_iterate(&temporary.anchor, placement, tile_placement_t, link) {
            list_remove(&temporary, &placement->link);
            list_insert_tail(cross, &placement->link);
        }
        return 1;
    }
    return 0;
}

static inline int apply_scorer(list_t *placed, size_t dim, board_state_unit_t *played[dim][dim], size_t  accumulated) {
    list_t primary;
    list_init(&primary);
    int sum = 0;
    list_iterate(&placed->anchor, placement, enriched_tile_placement_t, link) {
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

    list_iterate(&placements->anchor, placement, tile_placement_t, link) {
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
        total += BINGO;
    }
    return total;
}

void generator_clean_up(generation_result_t *result) {
    for (int c = 0; c < result->count; ++c) {
        candidate_t *candidate = result->candidates[c];
        DONE(candidate->serialized);
        DONE(candidate->placements);
        DONE(candidate);
    }
    DONE(result->candidates);

    list_iterate(&result->allocated_tile_placements.anchor, placement, tile_placement_t, result_link) {
        DONE(placement);
    }

    list_iterate(&result->allocated_tiles.anchor, tile, tile_t, result_link) {
        DONE(tile);
    }

    DONE(result);
}