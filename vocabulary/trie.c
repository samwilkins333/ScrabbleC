#include "trie.h"

inline trie_t *trie_initialize() {
    trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
    trie->root = trie_node_add_child_to(NULL, '@', 0);
    trie->word_count = 0;
    trie->node_count = 0;
    return trie;
}

inline unsigned short trie_add_nodes(trie_t *trie, const char *word) {
    int i = 0;
    unsigned short isTerminal = 0;
    trie_node_t *node = trie->root;
    trie_node_t *child_node;
    char letter;
    while ((letter = word[i++]) != '\0') {
        isTerminal = word[i] == '\0';
        if (!(child_node = trie_node_get_child(node, letter))) {
            node = trie_node_add_child_to(node, letter, isTerminal);
            trie->node_count++;
        } else {
            if ((isTerminal = isTerminal && !child_node->is_terminal)) {
                child_node->is_terminal = 1;
            }
            node = child_node;
        }
    }
    return isTerminal;
}

inline void trie_add_word(trie_t *self, const char *word) {
    if (trie_add_nodes(self, word)) {
        self->word_count++;
    }
}

inline int trie_includes_word(trie_t *self, const char *word) {
    trie_node_t *node = self->root;
    trie_node_t *child_node;
    char letter;
    int i = 0;
    while ((letter = word[i++]) != '\0') {
        if (!(child_node = trie_node_get_child(node, letter))) {
            break;
        }
        node = child_node;
    }
    return letter == '\0' && node->is_terminal;
}

inline void trie_destroy(trie_t *trie) {
    trie_node_destroy(trie->root);
}