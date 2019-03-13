#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int dimension = 0;
    char **board;

    srand(time(0));

    printf("Dimension of board: ");
    scanf("%d", &dimension);

    board = malloc(dimension * sizeof(char *));
    for (int i = 0; i < dimension; i++) {
        board[i] = malloc(dimension * sizeof(char));
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
             board[i][j] = 65 + (rand() % 26);
            printf("%c", board[i][j]);
        }
        printf("\n");
    } 

    return 0;
}