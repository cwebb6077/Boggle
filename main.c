#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int main(void) {

    int dimension = 0;
    int numPlayers = 0;
    char **board;

    printf("Dimension of board: ");
    scanf("%d", &dimension);

    printf("Possible Game Modes:\n(1) Player vs. Computer\n(2) Player vs. Player\n");
    printf("Select Number of Players: ");
    scanf("%d", &numPlayers);

    switch(numPlayers) {
        case 1:

            // allocates memory globally for the board 
            board = malloc(dimension * sizeof(char *));
            for (int i = 0; i < dimension; i++) {
                board[i] = malloc(dimension * sizeof(char));
            }

            create_board(dimension, board);
            break;

        case 2:
        
            // allocates memory globally for the board 
            board = malloc(dimension * sizeof(char *));
            for (int i = 0; i < dimension; i++) {
                board[i] = malloc(dimension * sizeof(char));
            }

            create_board(dimension, board);
            break;
    }

    //free_board(dimension, board);
    
    // deallocates memory of board
    for (int i = 0; i < dimension; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}