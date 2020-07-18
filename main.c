#include "vocabulary/trie_factory.h"

int main() {
    trie_t *trie = construct_trie_from("../ospd4.txt");
    if (!trie) {
        return 1;
    }
    printf("%ld words\n", trie->word_count);
    printf("%ld nodes\n", trie->node_count);
    trie_destroy(trie);
    return 0;
}