#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"

struct trieNode {
    struct trieNode *children[26];
    bool endOfWord;
};

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
        if (!currNode->children[index]) currNode->children[index] = getNode();
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

int main(void) {

    int dimension = 0;
    int numPlayers = 0;
    //int numWins = 0; // num of Player 1 wins
    //int numTies = 0;
    //int numLoss = 0; // num of Player 2 wins
    char command[10];
    char **board;
    struct trieNode *root = getNode();
    char keys[][10] = {"the", "quick", "red", "fox", "jumped", "over", "the" , "lazy", "brown", "dog"};
    for (int i = 0; i < 10; i++){
        insert(root, keys[i]);
    }
    printf("Enter word to check if they are in trie\n");
    scanf("%s", command);
    int temp = search(root, command);
    if (temp == 0) printf("No\n");
    else printf("Yes\n");

    printf("\n\n\n***** Boggle *****\n\n\n");
    printf("Dimension of board: ");
    scanf("%d", &dimension);

    printf("\nPossible Game Modes:\n<1> Player vs. Computer\n<2> Player vs. Player\n");
    printf("Select Number of Players: ");
    scanf("%d", &numPlayers);

    printf("\n\n");
    
    strcpy(command, "");
    switch(numPlayers) {
        case 1:

            board = malloc(dimension * sizeof(char *));
            create_board(dimension, board);
            
            break;

        case 2:
            while(strcmp(command, "!quit")) {

                board = malloc(dimension * sizeof(char *));
                create_board(dimension, board);               
                
                printf("Player 1: <Type !pass to end turn>\n");
                while(strcmp(command,"!pass")){
                    scanf("%s", command);
                }

                strcpy(command, "");
                printf("Player 2: <Type !pass to end turn>\n");
                while(strcmp(command, "!pass")){
                    scanf("%s", command);
                }

                free_board(dimension, board);
                free(board);

                strcpy(command, "");
                printf("Do you wish to continue? <Yes or No>\n");
                scanf("%s", command);
                if (!strcmp(command, "No")) break;
            }
            break;
    }

    return 0;
}

