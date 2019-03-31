#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "board.h"
#include "trie.h"

/* Written by: Connor Webb
   This program implements the game of Boggle. The game can either be played against the computer or against a human opponent.
   A dictionary is loaded into a trie data structure for quick access. 
   The computer uses Depth First Searches to find all the words on the board.
   The user is given 3 minutes to look for words, and then must type them in one at a time.
*/

// NEED TO DO: Change str func's to strn func's
//             Test for bugs
//             Make curses interface

void dfs(char **board, struct trieNode *root, struct trieNode *temp, struct trieNode *wordsInBoard, int **discovered, char *wordToAdd, int dimension, int i, int j);

int main(void) {

    int dimension = 0;
    int numPlayers = 0;
    int p1points = 0; // points for Player 1 in a game
    int p2points = 0; // points for Player 2 in a game
    int numWins = 0; // num of Player 1 wins
    int numTies = 0;
    int numLoss = 0; // num of Player 2 wins
    char command[50];
    command[0] = '\0';
    time_t startTurn, difference;
    int duration = 20; //this sets the duration of a turn as 3 minutes (written in secs) 180
    char **board;
    int **discovered;
    char wordToAdd[20];
    wordToAdd[0] = '\0';
    struct trieNode *root = getNode(); // trie for all of the words in the dictionary

    FILE *wordFile = fopen("dict.txt", "r");
    while(!feof(wordFile)) {
        fscanf(wordFile, "%s", command);
        insert(root, command);
    }
    fclose(wordFile);

    initscr();
    cbreak();
    noecho();
    printw("\n\n\n***** Boggle *****\n\n\n");
    









    printw("\nPossible Game Modes:\n<1> Player vs. Computer\n<2> Player vs. Player\n");
    refresh();
    printw("Select Number of Players: ");
    refresh();
    getch();
    endwin();
    //scanf("%d", &numPlayers);
    
    printf("\n\n");

    strncpy(command, "", 1);
    switch(numPlayers) {
        case 1:
            while(1) {

                printf("Dimension of board: <Entered as <N> for an N x N board>\n");
                scanf("%d", &dimension);

                if (dimension > 85) {
                    printf("Warning: Boards larger than 85 tend to wrap around and be hard to read.\n");
                    printf("Do you wish to continue? <Yes or No>\n");
                    scanf("%s", command);
                    if (!strncmp(command, "No", 50)) {
                        deleteTrie(root);
                        return -1;
                    }
                }

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

                // run DFS for every cell on the board to find all of the words
                startTurn = time(0);
                struct trieNode *temp = root;
                for (int i = 0; i < dimension; i++) {
                    for (int j = 0; j < dimension; j++) {
                        dfs(board, root, temp, wordsInBoard, discovered, wordToAdd, dimension, i, j);
                        strncpy(wordToAdd, "", 1);
                    }
                }   
                difference = (time(0) - startTurn); //computes time elapsed that was used for searching
                if (difference < duration) {
                    while (difference < duration) {
                        difference = (time(0) - startTurn); // stay in while loop until the full 3 minutes is passed
                    }
                } 

                // free the dimension matrix
                for (int i = 0; i < dimension; i++) {
                    free(discovered[i]);
                }
                free(discovered);            
                
                printf("Player 1: <Type !pass to end turn>\n");
                do {
                    scanf("%s", command);
                    if (!strncmp(command, "!pass", 50)) break;
                    // assign points for the word based on the rules of the game
                    if (search(wordsInBoard, command) == 1) { // first search if it is an acceptable word
                        if (strlen(command) == 3 || strlen(command) == 4) p1points++;
                        else if (strlen(command) == 5) p1points += 2;
                        else if (strlen(command) == 6) p1points += 3;
                        else if (strlen(command) == 7) p1points += 5;
                        else if (strlen(command) >= 8) p1points += 11;
                    }
                } while (strcmp(command, "!pass"));
                
                strcpy(command, ""); //resets commands so that previous command may not be reused
                printf("Computer: \n");

                char str[20];
                p2points = displayTrieWithScore(wordsInBoard, str, 0, 0); // display the words found by the computer and compute score for each

                // free some memory
                deleteTrie(wordsInBoard);
                free_board(dimension, board);
                free(board);

                // compare scores and assign match scores
                printf("The point total for this game is:\n\nPlayer 1: %d\nComputer: %d\n\n", p1points, p2points);
                if (p1points > p2points) {
                    printf("Player 1 wins this game.\n\n");
                    ++numWins;
                }
                else if (p1points == p2points) {
                    printf("This game is a draw.\n\n");
                    ++numTies;
                }
                else {
                    printf("Computer wins this game.\n\n");
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

                printf("Dimension of board: ");
                scanf("%d", &dimension);

                if (dimension > 85) {
                    printf("Warning: Boards larger than 85 tend to wrap around and be hard to read.\n");
                    printf("Do you wish to continue? <Yes or No>\n");
                    scanf("%s", command);
                    if (!strncmp(command, "No", 50)) {
                        deleteTrie(root);
                        return -1;
                    }
                }

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

                // run DFS for every cell on the board to find all of the words
                startTurn = time(0);
                struct trieNode *temp = root;
                for (int i = 0; i < dimension; i++) {
                    for (int j = 0; j < dimension; j++) {
                        dfs(board, root, temp, wordsInBoard, discovered, wordToAdd, dimension, i, j);
                        strncpy(wordToAdd, "", 1);
                    }
                }   
                difference = (time(0) - startTurn); //computes time elapsed for searching
                if (difference < duration) {
                    while (difference < duration) {
                        difference = (time(0) - startTurn);
                    }
                }

                // free the dimension matrix
                for (int i = 0; i < dimension; i++) {
                    free(discovered[i]);
                }
                free(discovered);            
                
                printf("Player 1: <Type !pass to end turn>\n");
                do {
                    scanf("%s", command);
                    if (!strncmp(command, "!pass", 50)) break;
                    // assign points for the word based on the rules of the game
                    if (search(wordsInBoard, command) == 1) { // first search if it is an acceptable word
                        if (strlen(command) == 3 || strlen(command) == 4) p1points++;
                        else if (strlen(command) == 5) p1points += 2;
                        else if (strlen(command) == 6) p1points += 3;
                        else if (strlen(command) == 7) p1points += 5;
                        else if (strlen(command) >= 8) p1points += 11;
                    }
                    
                } while (strcmp(command, "!pass"));
                
                strcpy(command, ""); //resets commands so that previous command may not be reused
                printf("Player 2: <Type !pass to end turn>\n");
                do {
                    scanf("%s", command);
                    if (!strncmp(command, "!pass", 50)) break;
                    if (search(wordsInBoard, command) == 1) {
                        if (strlen(command) == 3 || strlen(command) == 4) p2points++;
                        else if (strlen(command) == 5) p2points += 2;
                        else if (strlen(command) == 6) p2points += 3;
                        else if (strlen(command) == 7) p2points += 5;
                        else if (strlen(command) >= 8) p2points += 11;
                    }
                } while (strcmp(command, "!pass"));

                deleteTrie(wordsInBoard);
                free_board(dimension, board);
                free(board);

                printf("The point total for this game is:\n\nPlayer 1: %d\nPlayer 2: %d\n\n", p1points, p2points);
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
        default:
            printf("Entered wrong choice\n");
            break;
    }
    deleteTrie(root);
    //endwin();
    return 0;

}

// The following is an algorithm to search the board based on Depth First Search
// Either Depth First or Breadth First Search could be used, they would perform equivalently
// The general idea came from the pseudocode found in the book
// The main modification is checking to see if the node is actually an acceptable letter (see further down for this)
void dfs(char **board, struct trieNode *root, struct trieNode *temp, struct trieNode *wordsInBoard, int **discovered, char *wordToAdd, int dimension, int i, int j) {

    if (i < 0 || j < 0 || i >= dimension || j >= dimension) return; // stops from going outside of the board
    if (discovered[i][j]) return; // makes sure cell hasn't been visited

    discovered[i][j] = 1; // sets cell to discovered
   
    // add the new letter to the possible word and search trie to see if it is contained in it, if not continue searching
    strncat(wordToAdd, &board[i][j], 1);
    if (search(root, wordToAdd) == 1) {
        insert(wordsInBoard, wordToAdd);
    }

    // first, we need to check to see if the new letter is actually one that would lead to a new word 
    // (e.g. if word = "trus", then newletter = 's' is okay, but newletter = 'x' is not)
    // this allows for the number of DFS searches to be greatly decreased (and therefore runtime decreased)
    // without this check, the runtime performance of an 11x11 board is >30 minutes because it checks every possible permutation of DFS
    // with it, the runtime reduces to a mere seconds
    // This can be done by traversing the tree downwards and then checking
    for (int k = 0; k < 26; k++) {
        if (temp->children[k] != NULL && k == board[i][j] - 97) {
            // this recursively calls DFS for all of the adjacent cells (the names are compass directions for simplicity)
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i - 1, j); // cell directly north
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i - 1, j + 1); // cell directly northeast
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i, j + 1); // cell directly east
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i + 1, j + 1); // cell directly southeast
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i + 1, j); // cell directly south
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i + 1, j - 1); // cell directly southwest
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i, j - 1); // cell directly west
            dfs(board, root, temp->children[k], wordsInBoard, discovered, wordToAdd, dimension, i - 1, j - 1); // cell directly northwest
        }
    }

    // once we are done, we take the last letter off, set cell to not discovered, and return
    wordToAdd[strlen(wordToAdd) - 1] = '\0';
    discovered[i][j] = 0;
    return;
}
