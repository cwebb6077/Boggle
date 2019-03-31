#ifndef _trie_h
#define _trie_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

struct trieNode {
    struct trieNode *children[26];
    int endOfWord;
};

struct trieNode *getNode(void);
void insert(struct trieNode *root, char *key);
int search(struct trieNode *root, char *key);
int displayTrieWithScore(struct trieNode *root, char str[], int level, int points);
void deleteTrie(struct trieNode *currNode);

#endif
