#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "trie.h"

int main(void) {

    int dimension = 0;
    int numPlayers = 0;
    //int numWins = 0; // num of Player 1 wins
    //int numTies = 0;
    //int numLoss = 0; // num of Player 2 wins
    char command[50];
    char **board;
    struct trieNode *root = getNode();

    FILE *wordFile = fopen("dict.txt", "r");
    while(!feof(wordFile)) {
        fscanf(wordFile, "%s", command);
        insert(root, command);
    }
    fclose(wordFile);

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
            while(1) {

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
    deleteTrie(root);

    return 0;
}

