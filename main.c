#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "trie.h"

int main(void) {

    int dimension = 0;
    int numPlayers = 0;
    int p1points = 0; // points for Player 1 in a game
    int p2points = 0; // points for Player 2 in a game
    int numWins = 0; // num of Player 1 wins
    int numTies = 0;
    int numLoss = 0; // num of Player 2 wins
    char command[50];
    time_t startTurn, difference;
    int duration = 180; //this sets the duration of a turn as 3 minutes (written in secs)
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
                startTurn = time(0);
                do {
                    scanf("%s", command);
                    // assign points for the word based on the rules of the game
                    if (search(root, command) == 1) { // first search if it is an acceptable word
                        if (strlen(command) == 3 || strlen(command) == 4) p1points++;
                        else if (strlen(command) == 5) p1points += 2;
                        else if (strlen(command) == 6) p1points += 3;
                        else if (strlen(command) == 7) p1points += 5;
                        else if (strlen(command) >= 8) p1points += 11;
                    }
                    difference = (time(0) - startTurn); //computes time elapsed during turn
                } while (strcmp(command, "!pass") && difference <= duration);
                //p1points -= 2; // subtracts the score for "!pass"
                
                strcpy(command, ""); //resets commands so that previous command may not be reused
                printf("Player 2: <Type !pass to end turn>\n");
                startTurn = time(0);
                do {
                    scanf("%s", command);
                    if (search(root, command) == 1) {
                        if (strlen(command) == 3 || strlen(command) == 4) p2points++;
                        else if (strlen(command) == 5) p2points += 2;
                        else if (strlen(command) == 6) p2points += 3;
                        else if (strlen(command) == 7) p2points += 5;
                        else if (strlen(command) >= 8) p2points += 11;
                    }
                    difference = (time(0) - startTurn);
                } while (strcmp(command, "!pass") && difference <= duration);
                //p2points -= 2;

                free_board(dimension, board);
                free(board);

                printf("The point total for this game is:\n\nPlayer 1: %d\nPlayer2: %d\n\n", p1points, p2points);
                if (p1points > p2points) {
                    printf("Player 1 wins this game.\n\n");
                    ++numWins;
                }
                else if (p1points == p2points) {
                    printf("This game is a draw.\n\n");
                    ++numTies;
                }
                else {
                    printf("Player 2 wins this game.\n\n");
                    ++numLoss;
                }
                p1points = 0;
                p2points = 0;

                printf("Current Score: %d - %d - %d\n\n", numWins, numTies, numLoss);

                strcpy(command, "");
                printf("Do you wish to continue? <Yes or No>\n");
                scanf("%s", command);
                if (!strcmp(command, "No")) break;
            }
            printf("\nFinal Score: %d - %d - %d\n\n", numWins, numTies, numLoss);
            break;
    }
    deleteTrie(root);

    return 0;
}

