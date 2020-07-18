#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct trie_node {
    char letter;
    char isTerminal;
    struct trie_node *parent;
    struct trie_node *children[26];
} trie_node_t;

typedef struct trie {
    trie_node_t *root;
    void (*add_word)(char *word);
} trie_t;

static inline trie_node_t *initialize_node(char letter, char isTerminal, trie_node_t *parent) {
    trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
    memset(node->children, 0, sizeof(node->children));
    node->letter = letter;
    node->isTerminal = isTerminal;
    node->parent = parent;
    if (parent != 0) {
        parent->children[letter] = node;
    }
    return node;
}

static inline void destroy_node(trie_node_t *node) {
    if (node) {
        for (int i = 0; i < 26; ++i) {
            destroy_node(node->children[i]);
        }
        free(node);
    }
}

static inline trie_node_t *get_child(trie_node_t *node, char letter) {
    return node->children[letter - 97];
}

static void trie_add_word(char *word) {
    printf("%s", word);
}

static inline trie_t *initialize_trie() {
    trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
    trie->root = initialize_node('@', 0, NULL);
    trie->add_word = trie_add_word;
    return trie;
}

static inline void destroy_trie(trie_t *trie) {
    destroy_node(trie->root);
}