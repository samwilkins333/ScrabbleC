#include <stdio.h>
#include <zconf.h>
#include "trie.h"

#define OPEN\
    dictionary = fopen("../ospd4.txt", "r");\
    if (dictionary == NULL) {\
        perror("Failed to read dictionary.");\
        return 1;\
    }\


#define ITERATE\
    while (getline(&word, &len, dictionary) != -1) {\
        word[strcspn(word, "\n")] = 0;\


#define CLOSE fclose(dictionary);

int main() {
    FILE *dictionary;
    char *word;
    int failed = 0;
    size_t len = 0;

    trie_t *trie = trie_initialize();
    OPEN;
    ITERATE
        trie->add_word(trie, word);
    }
    CLOSE;

    printf("Trie contains %ld words and %ld nodes\n", trie->word_count, trie->node_count);

    OPEN;
    ITERATE
        if (!trie->includes(trie, word)) {
            printf("%s was not correctly added to the trie.\n", word);
            failed++;
        }
    }
    if (!failed) {
        printf("Successfully validated all words.\n");
    }
    CLOSE;

    trie_destroy(trie);

    return 0;
}