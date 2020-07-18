#include <stdio.h>
#include <zconf.h>
#include "trie.h"

int main() {
    FILE *dictionary = fopen("../ospd4.txt", "r");
    if (dictionary == NULL) {
        perror("Failed to read dictionary");
        return 1;
    }
    size_t len = 0;
    char *word;
    trie_t *trie = initialize_trie();
    while (getline(&word, &len, dictionary) != -1) {
        trie->add_word(word);
    }
    trie_node_t *node = initialize_node('c', 0, NULL);
    printf("%c\n", node->letter);
    return 0;
}