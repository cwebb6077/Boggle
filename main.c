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
   The UI for this program is ncurses.
   The user is given 3 minutes to look for words, and then must type them in one at a time.
*/

void dfs(char **board, struct trieNode *root, struct trieNode *temp, struct trieNode *wordsInBoard, int **discovered, char *wordToAdd, int dimension, int i, int j);

int main(void) {

    int dimension = 0;
    int p1points = 0; // points for Player 1 in a game
    int p2points = 0; // points for Player 2 in a game
    int numWins = 0; // num of Player 1 wins
    int numTies = 0;
    int numLoss = 0; // num of Player 2 wins
    char command[50];
    command[0] = '\0';
    time_t startTurn, difference;
    int duration = 180; //this sets the duration of a turn as 3 minutes (written in secs) 180
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
    scrollok(stdscr, true);

    printw("************** Boggle **************\n");
    printw("Possible Game Modes");

    // This makes the start menu for the game modes using ncurses functions
    WINDOW *menu = newwin(6, 22, 3, 6);
    refresh();
    wrefresh(menu);
    keypad(menu, true);
    char choices[2][20] = {"Player vs. Computer", "Player vs. Player"};
    int choice;
    int highlight = 0;
    while(1) {
        for (int i = 0; i < 2; i++) {
            if (i == highlight) {
                wattron(menu, A_REVERSE);// highlight the area

            }
            mvwprintw(menu, i + 1, 1, choices[i]);
            wattroff(menu, A_REVERSE);
        }
        choice = wgetch(menu);

        switch(choice) {
            case KEY_UP:
                highlight--;
                if (highlight == -1) highlight = 0; // do not go off menu 
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == 2) highlight = 1; // do not go off menu
                break;
            default:
                break;
        }
        if (choice == 10) break;
    }

    clear();
    
    //Print the rules so that players know how to play the game
    printw("Rules:\n\n");
    printw("- The players receive 3 minutes to find words in the board.\n");
    printw("- A tile cannot be reused when spelling a word.\n");
    printw("- The player will be prompted to enter words. They must be English words less than 50 letters long.\n");
    printw("- To end a turn, the player must type '!pass'\n");
    printw("- The scoring of words is as follows:\n");
    printw("   - 1 point for words of length 3 or 4\n");
    printw("   - 2 points for words of length 5\n");
    printw("   - 3 points for words of length 6\n");
    printw("   - 5 points for words of length 7\n");
    printw("   - 11 points for words of length 8 or more\n");
    printw("\n\n\nPress any key to continue");
    getch(); // this waits for any key to be pressed

    strncpy(command, "", 1);

    while(1) {

        clear();
        printw("Dimension of board: <Entered as <N> for an N x N board>\n");
        refresh();
        scanw("%d", &dimension);

        // warnings for board sizes if too large or two small
        // sets up menus for dealing with these
        if (dimension > 85 || dimension < 4) {
            clear();
            if (dimension > 85) {
                start_color();
                init_pair(1, COLOR_WHITE, COLOR_RED);
                attron(COLOR_PAIR(1)); 
                printw("Warning: Boards larger than 85x85 tend to wrap around and be hard to read.\n");
                attroff(COLOR_PAIR(1));
            }
            if (dimension < 4) {
                start_color();
                init_pair(1, COLOR_WHITE, COLOR_RED);
                attron(COLOR_PAIR(1));
                printw("Warning: Boards smaller than 4x4 tend to not have words in them\n");
                attroff(COLOR_PAIR(1));
            }
            printw("Do you wish to continue?\n");
            WINDOW *helpmenu = newwin(6, 12, 3, 6);
            refresh();
            wrefresh(helpmenu);

            keypad(helpmenu, true);

            char helpChoices[2][5] = {"Yes", "No"};
            int helpChoice;
            int helpHighlight = 0;

            while (1) {
                for (int i = 0; i < 2; i++) {
                    if (i == helpHighlight) {
                        wattron(helpmenu, A_REVERSE);
                    }
                    mvwprintw(helpmenu, i + 1, 1, helpChoices[i]);
                    wattroff(helpmenu, A_REVERSE);
                }
                helpChoice = wgetch(helpmenu);
                switch(helpChoice) {
                    case KEY_UP:
                        helpHighlight--;
                        if (helpHighlight == -1) helpHighlight = 0;
                        break;
                    case KEY_DOWN:
                        helpHighlight++;
                        if (helpHighlight == 2) helpHighlight = 1;
                        break;
                    default:
                        break;
                }
                if (helpChoice == 10) break;
            }
            if (helpHighlight == 1) {
                endwin();
                deleteTrie(root);
                return -1;
            }
        } 

        clear();
        refresh();
        endwin();
        board = malloc(dimension * sizeof(char *));
        create_board(dimension, board);
        printf("\n\n\n\n\n");
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

        clear();

        switch(highlight) {
            case 0: // player vs. computer mode
                printw("Player 1: <Type !pass to end turn>\n");
                refresh();
                do {
                    scanw("%s", command);
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
                
                strncpy(command, "", 50); //resets commands so that previous command may not be reused
                clear();
                printw("Computer: \n");
                refresh();

                char str[20];
                p2points = displayTrieWithScore(wordsInBoard, str, 0, 0); // display the words found by the computer and compute score for each
                printw("Press any key to continue.");
                refresh();
                getch(); 

                // free some memory
                deleteTrie(wordsInBoard);
                free_board(dimension, board);
                free(board);

                // compare scores and assign match scores
                clear();
                printw("The point total for this game is:\n\nPlayer 1: %d\nComputer: %d\n\n", p1points, p2points);
                refresh();
                if (p1points > p2points) {
                    printw("Player 1 wins this game.\n\n");
                    ++numWins;
                }
                else if (p1points == p2points) {
                    printw("This game is a draw.\n\n");
                    ++numTies;
                }
                else {
                    printw("Computer wins this game.\n\n");
                    ++numLoss;
                }
                p1points = 0;
                p2points = 0;

                
            break;

        case 1: // player vs. player mode
           printw("Player 1: <Type !pass to end turn>\n");
           refresh();
            do {
                scanw("%s", command);
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
                
            strncpy(command, "", 50); //resets commands so that previous command may not be reused
            clear();
            printw("Player 2: <Type !pass to end turn>\n");
            refresh();
            do {
                scanw("%s", command);
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

            clear();
            printw("The point total for this game is:\n\nPlayer 1: %d\nPlayer 2: %d\n\n", p1points, p2points);
            refresh();
            if (p1points > p2points) {
                printw("Player 1 wins this game.\n\n");
                ++numWins;
            }
            else if (p1points == p2points) {
                printw("This game is a draw.\n\n");
                ++numTies;
            }
            else {
                printw("Player 2 wins this game.\n\n");
                ++numLoss;
            }
            p1points = 0;
            p2points = 0;

            break;
        default:
            break;
        }
        printw("Current Score: %d - %d - %d\n\n", numWins, numTies, numLoss);

        // make a new menu for choosing to quit the game or continue
        printw("New Game?\n");
        WINDOW *endmenu = newwin(6, 12, 10, 6);
        refresh();
        wrefresh(endmenu);
        keypad(endmenu, true);
        char endChoices[2][5] = {"Yes", "No"};
        int endChoice;
        int endHighlight = 0;
        while(1) {
            for (int i = 0; i < 2; i++) {
                if (i == endHighlight) {
                    wattron(endmenu, A_REVERSE);
                }
                mvwprintw(endmenu, i + 1, 1, endChoices[i]);
                wattroff(endmenu, A_REVERSE);
            }
            endChoice = wgetch(endmenu);

            switch(endChoice) {
                case KEY_UP:
                    endHighlight--;
                    if (endHighlight == -1) endHighlight = 0;
                    break;
                case KEY_DOWN:
                    endHighlight++;
                    if (endHighlight == 2) endHighlight = 1;
                    break;
                default:
                    break;
            }
            if (endChoice == 10) break;
        }
        if (endHighlight == 1) break;
        printf("\n\n\n\n\n");
    }
    endwin();
    printf("\nFinal Score: %d - %d - %d\n\n", numWins, numTies, numLoss); // print the final score and exit

    deleteTrie(root); // free dictionary trie from memory
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
    // (e.g. if word = "trus", then nextletter = 't' is okay, but nextletter = 'x' is not)
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
