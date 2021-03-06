#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct trie_node {
    char letter;
    unsigned short int is_terminal;
    struct trie_node *parent;
    struct trie_node *children[27];
} trie_node_t;

trie_node_t *trie_node_add_child_to(trie_node_t *node, char letter, unsigned short is_terminal);

trie_node_t *trie_node_get_child(trie_node_t *node, char letter);

void trie_node_destroy(trie_node_t *node);