#include "trie.h"

/* The general idea for implementing the trie data structure comes from https://www.geeksforgeeks.org/trie-insert-and-search/
The reason for using a trie is to improve the look-up time of words as compared to other data structures like hashmaps.
The look-up time with a trie is O(length of longest word) and most of the time is spent building the trie, not searching.
The only downfall is memory usage and a very large stack frame involved in creating a trie with such a large dataset. */

//This function allocates memory for each node and initializes all values to NULL/false
struct trieNode *getNode(void) {
    struct trieNode *newNode = NULL;
    newNode = (struct trieNode *)malloc(sizeof(struct trieNode));

    if (newNode) {
        newNode->endOfWord = 0;

        for (int i = 0; i < 26; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

//This function inserts a new word into the trie
void insert(struct trieNode *root, char *key){
    int index;

    struct trieNode *currNode = root;

    //traverses down the trie until the end of the word is reached
    for (int i = 0; i < strlen(key); i++) {
        index = key[i] - 97; //97 is the ASCII value for 'a'
        if (currNode->children[index] == NULL) {
            currNode->children[index] = getNode();
        }
        currNode = currNode->children[index]; 
    }
    currNode->endOfWord = 1;
}

//This function searches the trie for a given word
//It operates similarly to "insert" function above
int search(struct trieNode *root, char *key) {
    int index;

    struct trieNode *currNode = root;

    for (int i = 0; i < strlen(key); i++) {
        index = key[i] - 97;
        if (!currNode->children[index]) return 0; //basically, if the letter is not used in the node, the word is not in the trie
        currNode = currNode->children[index];
    }
    return (currNode != NULL && currNode->endOfWord); //the word can only be in the trie if we have reached the bottom and the endOfWord flag is true
}

void displayTrie(struct trieNode *root, char str[], int level) {
    if (root->endOfWord == 1) {
        str[level] = '\0';
        if (strlen(str) >= 3) {
            printf("%s\n", str);
        }
    }
    for (int i = 0; i < 26; i++) {
        if (root->children[i]) {
            str[level] = i + 97;
            displayTrie(root->children[i], str, level + 1);
        }
    }
}

int displayTrieWithScore(struct trieNode *root, char str[], int level) {
    int points = 0;
    if (root->endOfWord == 1) {
        str[level] = '\0';
        if (strlen(str) == 3 || strlen(str) == 4) {
            printf("%s\n", str);
            points++;
        } 
        else if (strlen(str) == 5){
            printf("%s\n", str);
            points += 2;
        }
        else if (strlen(str) == 6) {
            printf("%s\n", str);
            points += 3;
        } 
        else if (strlen(str) == 7) {
            printf("%s\n", str);
            points += 5;
        } 
        else if (strlen(str) >= 8) {
            printf("%s\n", str);
            points += 11;
        } 
    }
    for (int i = 0; i < 26; i++) {
        if (root->children[i]) {
            str[level] = i + 97;
            displayTrie(root->children[i], str, level + 1);
        }
    }
    return points;
}

//This function frees the memory of the trie when the main program is finished
//The operation is fairly straightforward, for each edge connected to the root, recursively free the subtries
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
