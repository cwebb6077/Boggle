#include "board.h"

void create_board(int dimension, char** board) {

    // initializes the seed for random number generator
    srand(time(0));

    // allocates memory globally for the board 
    board = malloc(dimension * sizeof(char *));
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
            if (temp < 7) board[i][j] = 'Z';
            else if (temp < 10) board[i][j] = 'J';
            else if (temp < 11) board[i][j] = 'Q';
            else if (temp < 17) board[i][j] = 'X';
            else if (temp < 69) board[i][j] = 'K';
            else if (temp < 111) board[i][j] = 'V';
            else if (temp < 149) board[i][j] = 'B';
            else if (temp < 182) board[i][j] = 'P';
            else if (temp < 203) board[i][j] = 'G';
            else if (temp < 209) board[i][j] = 'W';
            else if (temp < 211) board[i][j] = 'Y';
            else if (temp < 230) board[i][j] = 'F';
            else if (temp < 261) board[i][j] = 'M';
            else if (temp < 271) board[i][j] = 'C';
            else if (temp < 288) board[i][j] = 'U';
            else if (temp < 398) board[i][j] = 'L';
            else if (temp < 432) board[i][j] = 'D';
            else if (temp < 592) board[i][j] = 'H';
            else if (temp < 602) board[i][j] = 'R';
            else if (temp < 628) board[i][j] = 'S';
            else if (temp < 695) board[i][j] = 'N';
            else if (temp < 731) board[i][j] = 'I';
            else if (temp < 768) board[i][j] = 'O';
            else if (temp < 812) board[i][j] = 'A';
            else if (temp < 910) board[i][j] = 'T';
            else if (temp < 1202) board[i][j] = 'E';
            printf("%c ", board[i][j]);
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
    free(board);
    return;
}