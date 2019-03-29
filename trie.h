#ifndef _trie_h
#define _trie_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trieNode {
    struct trieNode *children[26];
    int endOfWord;
};

struct trieNode *getNode(void);
void insert(struct trieNode *root, char *key);
int search(struct trieNode *root, char *key);
void displayTrie(struct trieNode *root, char str[], int level);
int displayTrieWithScore(struct trieNode *root, char str[], int level);
void deleteTrie(struct trieNode *currNode);

#endif
