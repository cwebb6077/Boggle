#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "trie.h"


// NEED TO DO: Change str func's to strn func's

void dfs(char **board, struct trieNode *root, struct trieNode *wordsInBoard, int **discovered, char *wordToAdd, int dimension, int row, int col) {
 
    if (root->endOfWord == 1) insert(wordsInBoard, wordToAdd); // if a word is found, it will be added to the trie for the words in the board
    if (row >= 0 && col >= 0 && row < dimension && col < dimension && !discovered[row][col]) {
        
        discovered[row][col] = 1;

        for (int i = 0; i < 26; i++) {
            if (root->children[i] != NULL) {

                char temp[2];
                temp[0] = i + 97; // converts i into letter
                strcat(wordToAdd, temp); 

                // the following block of code recursively calls a dfs on each nonvisited cell adjacent to the starting cell 
                // (I am going to represent these cells by compass directions for simplicity)
                if (row - 1 >= 0 && col >= 0 && row - 1 < dimension && col < dimension && !discovered[row - 1][col]){ //cell directly north
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row - 1, col);
                }
                if (row - 1 >= 0 && col + 1 >= 0 && row - 1 < dimension && col + 1 < dimension && !discovered[row - 1][col + 1]){ //cell directly northeast
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row - 1, col + 1);
                }
                if (row >= 0 && col + 1 >= 0 && row < dimension && col + 1 < dimension && !discovered[row][col + 1]){ //cell directly east
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row, col + 1);
                }
                if (row + 1 >= 0 && col + 1 >= 0 && row + 1 < dimension && col + 1 < dimension && !discovered[row + 1][col + 1]){ //cell directly southeast
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row + 1, col + 1);
                }
                if (row + 1 >= 0 && col >= 0 && row + 1 < dimension && col < dimension && !discovered[row + 1][col]){ //cell directly south
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row + 1, col);
                }
                if (row + 1 >= 0 && col - 1 >= 0 && row + 1 < dimension && col - 1 < dimension && !discovered[row + 1][col - 1]){ //cell directly southwest
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row + 1, col - 1);
                }
                if (row >= 0 && col - 1 >= 0 && row < dimension && col - 1 < dimension && !discovered[row][col - 1]){ //cell directly west
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row, col - 1);
                }
                if (row - 1 >= 0 && col - 1 >= 0 && row - 1 < dimension && col - 1 < dimension && !discovered[row - 1][col - 1]){ //cell directly northwest
                    dfs(board, root->children[i], wordsInBoard, discovered, wordToAdd, dimension, row - 1, col - 1);
                } 
            }
        }
        discovered[row][col] = 0;
    }
}

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
    int **discovered;
    char *wordToAdd;
    struct trieNode *root = getNode(); // trie for all of the words in the dictionary

    FILE *wordFile = fopen("file.txt", "r");
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
    
    // FIXME: Remember to change timing. Both players should play at the same time!

    strcpy(command, "");
    switch(numPlayers) {
        case 1:
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
                
                strncpy(command, "", 50); //resets commands so that previous command may not be reused
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
                } while (strncmp(command, "!pass", 50) && difference <= duration);

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

                strncpy(command, "", 50);
                printf("Do you wish to continue? <Yes or No>\n");
                scanf("%s", command);
                if (!strcmp(command, "No")) break;
            }
            printf("\nFinal Score: %d - %d - %d\n\n", numWins, numTies, numLoss);
            break;

        case 2:
            while(1) {

                board = malloc(dimension * sizeof(char *));
                create_board(dimension, board);
                struct trieNode *wordsInBoard = getNode(); // creates trie for words on the board

                // this creates a matrix to keep track of which cell has been discovered or not
                // each cell is initialized to be not discovered (0)
                discovered = malloc(dimension * sizeof(int *));
                for (int i = 0; i < dimension; i++) {
                    discovered[i] = malloc(dimension * sizeof(int));
                }
                for (int i = 0; i < dimension; i++) {
                    for (int j = 0; j < dimension; j++) {
                        discovered[i][j] = 0;
                    }
                }

                wordToAdd = malloc(sizeof(char *));
                strcpy(wordToAdd, "");
                // run DFS for every cell on the board to find all of the words
                for (int i = 0; i < dimension; i++) {
                    for (int j = 0; j < dimension; j++) {
                        strcpy(wordToAdd, &board[i][j]);
                        dfs(board, root, wordsInBoard, discovered, wordToAdd, dimension, i, j);
                        strcpy(wordToAdd, "");
                    }
                }   

                // free the dimension matrix
                for (int i = 0; i < dimension; i++) {
                    free(discovered[i]);
                }
                free(discovered);
                free(wordToAdd);            
                
                printf("Player 1: <Type !pass to end turn>\n");
                startTurn = time(0);
                do {
                    scanf("%s", command);
                    // assign points for the word based on the rules of the game
                    if (search(wordsInBoard, command) == 1) { // first search if it is an acceptable word
                        if (strlen(command) == 3 || strlen(command) == 4) p1points++;
                        else if (strlen(command) == 5) p1points += 2;
                        else if (strlen(command) == 6) p1points += 3;
                        else if (strlen(command) == 7) p1points += 5;
                        else if (strlen(command) >= 8) p1points += 11;
                    }
                    difference = (time(0) - startTurn); //computes time elapsed during turn
                } while (strcmp(command, "!pass") && difference <= duration);
                
                strcpy(command, ""); //resets commands so that previous command may not be reused
                printf("Player 2: <Type !pass to end turn>\n");
                startTurn = time(0);
                do {
                    scanf("%s", command);
                    if (search(wordsInBoard, command) == 1) {
                        if (strlen(command) == 3 || strlen(command) == 4) p2points++;
                        else if (strlen(command) == 5) p2points += 2;
                        else if (strlen(command) == 6) p2points += 3;
                        else if (strlen(command) == 7) p2points += 5;
                        else if (strlen(command) >= 8) p2points += 11;
                    }
                    difference = (time(0) - startTurn);
                } while (strcmp(command, "!pass") && difference <= duration);

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

                strncpy(command, "", 50);
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

