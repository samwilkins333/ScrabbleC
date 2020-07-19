cmake_minimum_required(VERSION 3.13)
project(ScrabbleLogic C)

set(CMAKE_C_STANDARD 99)

add_executable(ScrabbleLogic main.c vocabulary/trie.h vocabulary/trie_node.h vocabulary/trie_factory.h generation/models.h generation/generator.c generation/configuration.h utilities/list.c utilities/list.h generation/generator.h vocabulary/trie_node.c vocabulary/trie.c vocabulary/trie_factory.c generation/models.c)