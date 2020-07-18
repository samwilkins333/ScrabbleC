#include <stdio.h>
#include <zconf.h>
#include "trie.h"

#define verify_word(word) printf("%s exists? (%d)", word, trie->includes(trie, word))

int main() {
    FILE *dictionary = fopen("../ospd4.txt", "r");
    if (dictionary == NULL) {
        perror("Failed to read dictionary");
        return 1;
    }
    size_t len = 0;
    char *word;
    trie_t *trie = trie_initialize();
    while (getline(&word, &len, dictionary) != -1) {
        word[strcspn(word, "\n")] = 0;
        trie->add_word(trie, word);
    }
    verify_word("fish");
    verify_word("asglsdl");
    trie_destroy(trie);
    return 0;
}