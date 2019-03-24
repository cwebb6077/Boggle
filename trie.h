#ifndef _trie_h
#define _trie_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct trieNode {
    struct trieNode *children[26];
    bool endOfWord;
};

struct trieNode *getNode(void);
void insert(struct trieNode *root, char *key);
bool search(struct trieNode *root, char *key);
bool isEmpty(struct trieNode* root);
void deleteTrie(struct trieNode *currNode);

#endif
