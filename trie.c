#include "trie.h"

struct trieNode *getNode(void) {
    struct trieNode *newNode = NULL;
    newNode = (struct trieNode *)malloc(sizeof(struct trieNode));

    if (newNode) {
        newNode->endOfWord = false;

        for (int i = 0; i < 26; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

void insert(struct trieNode *root, char *key){
    int index;

    struct trieNode *currNode = root;

    for (int i = 0; i < strlen(key); i++) {
        index = key[i] - 97;
        if (currNode->children[index] == NULL) {
            currNode->children[index] = getNode();
        }
        currNode = currNode->children[index]; 
    }
    currNode->endOfWord = true;
}

bool search(struct trieNode *root, char *key) {
    int index;

    struct trieNode *currNode = root;

    for (int i = 0; i < strlen(key); i++) {
        index = key[i] - 97;
        if (!currNode->children[index]) return false;
        currNode = currNode->children[index];
    }
    return (currNode != NULL && currNode->endOfWord);
}

bool isEmpty(struct trieNode* root) {
    for (int i = 0; i < 26; i++) {
        if (root->children[i]) return false;
    }
    return true;
}

void deleteTrie(struct trieNode *currNode) {
    for (int i = 0; i < 26; i++) {
        if (currNode->children[i] != NULL) {
            deleteTrie(currNode->children[i]);
        }
        else continue;
    }
    free(currNode);
    return;
}
