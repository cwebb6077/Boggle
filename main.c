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

    create_board(dimension, board);
    //free_board(dimension, board);
    
    // deallocates memory of board
    for (int i = 0; i < dimension; i++) {
        free(board[i]);
    }
    free(board);
    return 0;
}