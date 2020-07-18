#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct trie_node {
    char letter;
    unsigned short int isTerminal;
    struct trie_node *parent;
    struct trie_node *children[26];
    struct trie_node *(*get_child)(struct trie_node *self, const char letter);
    struct trie_node *(*add_child)(struct trie_node *self, const char letter, unsigned short isTerminal);
} trie_node_t;

typedef struct trie {
    trie_node_t *root;
    long word_count;
    long node_count;
    void (*add_word)(struct trie *self, const char *word);
    int (*includes)(struct trie *self, const char *word);
} trie_t;


static inline trie_node_t *trie_node_get_child_impl(trie_node_t *node, char letter);
static inline trie_node_t *trie_node_add_child_impl(trie_node_t *node, char letter, unsigned short isTerminal);

static inline void trie_add_word_impl(trie_t *self, const char *word);
static inline int trie_includes_word_impl(trie_t *self, const char *word);

static inline trie_node_t *trie_node_initialize(char letter, unsigned short isTerminal, trie_node_t *parent) {
    trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
    memset(node->children, 0, sizeof(node->children));
    node->letter = letter;
    node->isTerminal = isTerminal;
    node->parent = parent;
    node->add_child = trie_node_add_child_impl;
    node->get_child = trie_node_get_child_impl;
    if (parent != 0) {
        parent->children[letter] = node;
    }
    return node;
}

static inline trie_node_t *trie_node_get_child_impl(trie_node_t *node, const char letter) {
    return node->children[letter - 97];
}

static inline trie_node_t *trie_node_add_child_impl(trie_node_t *node, const char letter, unsigned short isTerminal) {
    return node->children[letter - 97] = trie_node_initialize(letter, isTerminal, node);
}

static inline void trie_node_destroy(trie_node_t *node) {
    if (node) {
        for (int i = 0; i < 26; ++i) {
            trie_node_destroy(node->children[i]);
        }
        if (node->parent) {
            node->parent->children[node->letter] = 0;
        }
        free(node);
    }
}

static inline unsigned short trie_add_nodes(trie_t *trie, const char *word) {
    int i = 0;
    unsigned short isTerminal = 0;
    trie_node_t *node = trie->root;
    trie_node_t *child_node;
    char letter;
    while ((letter = word[i++]) != '\0') {
        isTerminal = word[i] == '\0';
        if (!(child_node = node->get_child(node, letter))) {
            node = node->add_child(node, letter, isTerminal);
            trie->node_count++;
        } else {
            if ((isTerminal = isTerminal && !child_node->isTerminal)) {
                child_node->isTerminal = 1;
            }
            node = child_node;
        }
    }
    return isTerminal;
}

static inline void trie_add_word_impl(trie_t *self, const char *word) {
    if (trie_add_nodes(self, word)) {
        self->word_count++;
    }
}

static inline int trie_includes_word_impl(trie_t *self, const char *word) {
    trie_node_t *node = self->root;
    trie_node_t *child_node;
    char letter;
    int i = 0;
    while ((letter = word[i++]) != '\0') {
        if (!(child_node = node->get_child(node, letter))) {
            break;
        }
        node = child_node;
    }
    return letter == '\0' && node->isTerminal;
}

static inline trie_t *trie_initialize() {
    trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
    trie->root = trie_node_initialize('@', 0, NULL);
    trie->add_word = trie_add_word_impl;
    trie->includes = trie_includes_word_impl;
    trie->word_count = 0;
    trie->node_count = 0;
    return trie;
}

static inline void trie_destroy(trie_t *trie) {
    trie_node_destroy(trie->root);
}