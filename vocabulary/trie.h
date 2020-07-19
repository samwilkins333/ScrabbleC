#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie_node.h"

typedef struct trie {
    trie_node_t *root;
    long word_count;
    long node_count;
} trie_t;

trie_t *trie_initialize();
unsigned short trie_add_nodes(trie_t *trie, const char *word);
void trie_add_word(trie_t *self, const char *word);
int trie_includes_word(trie_t *self, const char *word);
void trie_destroy(trie_t *trie);