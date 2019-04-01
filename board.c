#include "board.h"

// This file contains the function for creating the game board and also freeing it from memory

void create_board(int dimension, char** board) {

    // initializes the seed for random number generator
    srand(time(0));

    // allocates memory globally for the board 
    for (int i = 0; i < dimension; i++) {
        board[i] = malloc(dimension * sizeof(char));
    }

    // the following builds the board based on the frequency of letters in the English language
    // the frequency was multiplied by 100 to retain accuracy 
    // the table used can be found at http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html
    int temp = 0;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            temp = rand() % 1202;
            if (temp < 7) board[i][j] = 'z';
            else if (temp < 10) board[i][j] = 'j';
            else if (temp < 11) board[i][j] = 'q';
            else if (temp < 17) board[i][j] = 'x';
            else if (temp < 69) board[i][j] = 'k';
            else if (temp < 111) board[i][j] = 'v';
            else if (temp < 149) board[i][j] = 'b';
            else if (temp < 182) board[i][j] = 'p';
            else if (temp < 203) board[i][j] = 'g';
            else if (temp < 209) board[i][j] = 'w';
            else if (temp < 211) board[i][j] = 'y';
            else if (temp < 230) board[i][j] = 'f';
            else if (temp < 261) board[i][j] = 'm';
            else if (temp < 271) board[i][j] = 'c';
            else if (temp < 288) board[i][j] = 'u';
            else if (temp < 398) board[i][j] = 'l';
            else if (temp < 432) board[i][j] = 'd';
            else if (temp < 592) board[i][j] = 'h';
            else if (temp < 602) board[i][j] = 'r';
            else if (temp < 628) board[i][j] = 's';
            else if (temp < 695) board[i][j] = 'n';
            else if (temp < 731) board[i][j] = 'i';
            else if (temp < 768) board[i][j] = 'o';
            else if (temp < 812) board[i][j] = 'a';
            else if (temp < 910) board[i][j] = 't';
            else if (temp < 1202) board[i][j] = 'e';
            printf("%c ", board[i][j]); // prints the board to terminal
        }
        printf("\n\n");
    } 
    return;
}

void free_board(int dimension, char **board) {
    // deallocates memory of board
    for (int i = 0; i < dimension; i++) {
        free(board[i]);
    }
    return;
}

