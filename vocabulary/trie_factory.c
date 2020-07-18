#include "trie_factory.h"

extern inline trie_t *construct_trie_from(const char *relative_filepath) {
    trie_t *trie = trie_initialize();
    if (!trie) {
        return NULL;
    }
    FILE *dictionary = fopen(relative_filepath, "r");
    if (dictionary == NULL) {
        perror("Failed to read dictionary.");
        return NULL;
    }
    size_t len;
    char *word;
    while (getline(&word, &len, dictionary) != -1) {
        word[strcspn(word, "\n")] = 0;
        trie_add_word(trie, word);
    }
    fclose(dictionary);
    return trie;
}