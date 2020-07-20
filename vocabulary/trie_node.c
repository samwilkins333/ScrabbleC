#include "trie_node.h"
#include "../generation/configuration.h"

inline trie_node_t *trie_node_add_child_to(trie_node_t *node, char letter, unsigned short is_terminal) {
    NEW(trie_node_t, child, 1);
    child->letter = letter;
    child->is_terminal = is_terminal;
    if ((child->parent = node) != 0) {
        node->children[letter - 97] = child;
    }
    return child;
}

inline trie_node_t *trie_node_get_child(trie_node_t *node, char letter) {
    return node->children[letter - 97];
}

inline void trie_node_destroy(trie_node_t *node) {
    if (node) {
        for (int i = 0; i < 27; ++i) {
            trie_node_destroy(node->children[i]);
        }
        if (node->parent) {
            node->parent->children[node->letter - 97] = NULL;
        }
        free(node);
    }
}