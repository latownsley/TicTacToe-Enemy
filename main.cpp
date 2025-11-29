#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <algorithm>
#include <random>
#include <ctime>  

// *******************
//      Prototypes
// *******************

void showGameBoard(std::array<std::string, 9>& gameboard);

void updateMove(std::string playerName, std::string marker, std::array<std::string, 9>& gameboard);

void playEnemyMove(std::array<std::string, 9>& gameboard, int index, std::string marker);

std::pair<std::string, bool> evaluteGameboard(std::array<std::string, 9>& gameboard);

bool checkEndGameConditions(std::array<std::string, 9>& gameboard, std::string playerOne, std::string playerTwo);

bool inputValidation(int input, std::array<std::string, 9>& gameboard);

bool noMovesLeft(std::array<std::string, 9>& gameboard);

bool isEmpty(std::array<std::string, 9>& gameboard, int index);

int findScore(std::array<std::string, 9>& gameboard);

int minimax(std::array<std::string, 9>& gameboard, int depth, bool isMaxPlayer);

int findBestMove(std::array<std::string, 9>& gameboard);

int findRandomIndex(std::array<std::string, 9>& gameboard);

// *****************
//        Main 
// *****************
int main() {
    
    // declare variables
    std::string playerOne; //holds player one's name
    std::string playerTwo; // holds AI's name
    std::array<std::string, 9> gameboard = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};
    bool gameOver = false;
    int difficulty = 0; // holds Enemy difficulty setting(0(easiest)-3(hardest))


    // get player info
    std::cout << "Enter Player One's name: ";
    std::cin >> playerOne;
    std::cout << "\n" << playerOne << " will use X." << "\n" << "\n";

    std::cout << "How difficult do you want the enemy to be? Enter a number between 0 (easiest) and 3 (hardest)" << "\n" << "\n";
    std::cin >> difficulty;

    if (difficulty == 3) {
        playerTwo = "HAL 9000";
    } else if (difficulty == 2) {
        playerTwo = "Gladys";
    } else if (difficulty == 1) {
        playerTwo = "Skynet";
    } else {
        playerTwo = "Agent Smith";
    }

    std::cout << "You will be playing against " << playerTwo;
    std::cout << "\n" << playerTwo << " will use O." << "\n" << "\n";

    // game loop
    while (gameOver == false) {
        showGameBoard(gameboard);
        std::cout << "\n" << "\n";

        // Player One's Move
        std::cout << "\n" << playerOne << "'s turn: " << "\n";
        updateMove(playerOne, "X", gameboard);

        gameOver = checkEndGameConditions(gameboard, playerOne, playerTwo);
        if (gameOver == true) {
            break;
        }

        showGameBoard(gameboard);
        std::cout << "\n" << "\n";

        // Enemy's Move
        std::cout << "\n" << playerTwo << "'s turn: " << "\n";
        int index;
        if (difficulty == 3 || difficulty == 2) {
            index = findBestMove(gameboard);
        } else{
            index = findRandomIndex(gameboard);
        }
        
        playEnemyMove(gameboard, index, "O");

        gameOver = checkEndGameConditions(gameboard, playerOne, playerTwo);

    }
    
    return 0;
}

// ******************
//     Functions 
// ******************


/**
 * @brief Evaluate gameboard for winning move
 * 
 * @param gameboard The array that holds the gameboard.
 * @return Pair of string of winning marker ("X" or "O") and a bool if the game was a draw or not
 */
std::pair<std::string, bool> evaluteGameboard(std::array<std::string, 9>& gameboard) {
    std::string winningMarker;
    bool won = false;
    
    // check for wins
    // rows
    if (gameboard[0] == gameboard[1] && gameboard[2] == gameboard[1]) {
        winningMarker = gameboard[0];
        won = true;
    }

    if (gameboard[3] == gameboard[4] && gameboard[5] == gameboard[3]) {
        winningMarker = gameboard[4];
        won = true;
    }

    if (gameboard[6] == gameboard[7] && gameboard[8] == gameboard[6]) {
        winningMarker = gameboard[7];
        won = true;
    }
    
    // columns
    if (gameboard[0] == gameboard[3] && gameboard[3] == gameboard[6]) {
        winningMarker = gameboard[0];
        won = true;
    }

    if (gameboard[1] == gameboard[4] && gameboard[7] == gameboard[4]) {
        winningMarker = gameboard[4];
        won = true;
    }

    if (gameboard[2] == gameboard[5] && gameboard[5] == gameboard[8]) {
        winningMarker = gameboard[8];
        won = true;
    }

    // diagonal
    if (gameboard[0] == gameboard[4] && gameboard[8] == gameboard[4]) {
        winningMarker = gameboard[4];
        won = true;
    }

    if (gameboard[2] == gameboard[4] && gameboard[6] == gameboard[4]) {
        winningMarker = gameboard[4];
        won = true;
    }

    return std::make_pair(winningMarker, won);
}

/**
 * @brief Find random index for easy AI moves
 * 
 * @param gameboard The array that holds the gameboard.
 * @return Int of index of a random move
 */
int findRandomIndex(std::array<std::string, 9>& gameboard) {
    bool validated = false;
    int randomIndex = -1;
    std::srand(time(NULL));

    while (validated == false) {
        randomIndex = (rand() % 9);
        if (isEmpty(gameboard, randomIndex)){
            validated = true;
        }
    }
    return randomIndex;
}

/**
 * @brief Calculate score of gameboard per Minimax algorithm
 * 
 * @param gameboard The array that holds the gameboard.
 * @return Int of score
 */
int findScore(std::array<std::string, 9>& gameboard) {
    std::pair<std::string, bool> results = evaluteGameboard(gameboard);

    // if no winner
    if (results.second == false) {
        return 0;
    }

    //if someone won, check winning mark and return score
    if (results.first == "X") {
        return -10;
    } else if (results.first == "O") {
        return +10;
    }

    return 0;
}

/**
 * @brief recurse Minimax algorithm for determining best move. Calculates possible moves to the end of the game.
 * 
 * @param gameboard The array that holds the gameboard.
 * @param depth holds the level of recursion
 * @param isMaxPlayer hold a bool that determines if maximizing player (player) or not (Enemy AI)
 * @return Int of the best numerical value for the move
 */
int minimax(std::array<std::string, 9>& gameboard, int depth, bool isMaxPlayer) {
    int score = findScore(gameboard);

    // if the game is won
    if (score == 10) {
        return score + depth;
    }

    if (score == -10) {
        return score - depth;
    }

    // if no more moves / draw
    if (noMovesLeft(gameboard) == true) {
        return 0;
    }

    // if player move (maxPlayer)
    if (isMaxPlayer == true) {
        int best = -1000;

        // traverse the gameboard
        for (int i = 0; i < 9; i++) {
            std::string holder;
            // check if cell is empty
            if (isEmpty(gameboard, i)) {
                // if it is empty, make the move
                holder = gameboard[i];
                gameboard[i] = "X";

                // recursive call for next move
                best = std::max(best, minimax(gameboard, depth +1, !isMaxPlayer));

                // undo board move
                gameboard[i] = holder;
            }
        }
        return best;
    }
    // if AI Enemy move (minPlayer)
    else {
        int best = 1000;

        // traverse the gameboard
        for (int i = 0; i < 9; i++) {
            std::string holder;
            // check if cell is empty
            if (isEmpty(gameboard, i)) {
                // if it is empty, make the move
                holder = gameboard[i];
                gameboard[i] = "O";

                // recursive call for next move
                best = std::min(best, minimax(gameboard, depth +1, !isMaxPlayer));

                // undo board move
                gameboard[i] = holder;
            }
        }
        return best;
    }
}

/**
 * @brief Find best move for AI Enemy
 * 
 * @param gameboard The array that holds the gameboard.
 * @return Int of index of the best move
 */
int findBestMove(std::array<std::string, 9>& gameboard) {
    int bestVal = -1000;
    int bestIndex = -1;     // holds index of best move

    // Traverse all cells, evaluate minimax function for
    // all empty cells and return the cell with best value.

    for (int i = 0; i < 9; i++) {
        if (isEmpty(gameboard, i)){
            std::string holder;

            // make move
            holder = gameboard[i];
            gameboard[i] = "O";

            // evaluate minimax for move (starting with player's move (set to true))
            int moveVal = minimax(gameboard, 0, true);

            // undo move
            gameboard[i] = holder;

            // if val of move is more than current best val, update
            if (moveVal > bestVal) {
                bestIndex = i;
                bestVal = moveVal;
            }

        }
    }

    return bestIndex;
}

void playEnemyMove(std::array<std::string, 9>& gameboard, int index, std::string marker) {
    gameboard[index] = marker;
}

/**
 * @brief Shows the gameboard
 * 
 * @param gameboard The gameboard
 * @return 
 */
void showGameBoard(std::array<std::string, 9>& gameboard) {
    for (int i = 0; i <= 8; i++){
        std::cout << gameboard[i];

        if (i == 2 || i ==5){
            std::cout << "\n";
            std::cout << "---------";
            std::cout << "\n";
        } else if (i == 8){
            std::cout << "\n";
        }
        else {
            std::cout << " | ";
        }
    }
}

/**
 * @brief Updates the gameboard with new move
 * 
 * @param playerName The player's name.
 * @param marker The symbol "X" or "O", depending on the player.
 * @param gameboard The gameboard
 * @return 
 */
void updateMove(std::string playerName, std::string marker, std::array<std::string, 9>& gameboard) {
    int position;  // holds array index to update
    std::string positionString;
    bool validated = false;

    // get new move
    while (validated == false) {
        std::cout << playerName << ", please enter a position number (0-8): "<< "\n";
        std::cin >> positionString;
        position = std::stoi(positionString);
        validated = inputValidation(position, gameboard);
    }
    

    // update gameboard
    gameboard[position] = marker;

}

/**
 * @brief Verifies user input.
 * 
 * @param input The user inputed integer.
 * @param gameboard The gameboard.
 * @return A bool representing if the input was validated or not.
 */
bool inputValidation(int input, std::array<std::string, 9>& gameboard) {
    if (gameboard[input] == "X" || gameboard[input] == "O") {
        std::cout << "\nThis spot has already been chosen. Please choose a new position.";
        return false;
    }

    return true;
}


/**
 * @brief Checks End Game Conditions.
 * 
 * @param gameboard The array that holds the gameboard.
 * @param playerOne The playerOne name string.
 * @return A bool representing if the game is over or not.
 */
bool checkEndGameConditions(std::array<std::string, 9>& gameboard, std::string playerOne, std::string playerTwo) {
    std::string winningMarker;
    bool won = false;
    std::pair<std::string, bool> evaluatedResults;
    
    evaluatedResults = evaluteGameboard(gameboard);
    winningMarker = evaluatedResults.first;
    won = evaluatedResults.second;

    // display win results if win found
    if (won == true) {
        
        if (winningMarker == "X"){
            showGameBoard(gameboard);
            std::cout << "\n" << "\n";
            std::cout << "\n" << playerOne << " wins!";
            std::cout << "\n" << "Game Over" << "\n";
        } 
        else {
            showGameBoard(gameboard);
            std::cout << "\n" << "\n";
            std::cout << "\n" << playerTwo << " wins!";
            std::cout << "\n" << "Game Over" << "\n";
        }

        return true;
    }
    

    // check for draw
    bool noNumbers = noMovesLeft(gameboard);

    if (noNumbers == true) {
        showGameBoard(gameboard);
        std::cout << "\n" << "\n";
        std::cout << "It's a Draw!";
        std::cout << "\n" << "Game Over";
        return true;
    }

    // default
    return false;
}

/**
 * @brief Checks if any moves left
 * 
 * @param gameboard The array that holds the gameboard.
 * @return False if any moves left, true if none left
 */
bool noMovesLeft(std::array<std::string, 9>& gameboard) {
    for (int i = 0; i < 9; i++) {
        if (isEmpty(gameboard, i)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Checks if a cell is empty (can move to cell)
 * 
 * @param gameboard The array that holds the gameboard.
 * @return True if empty, false if not empty
 */
bool isEmpty(std::array<std::string, 9>& gameboard, int index) {
    if (gameboard[index] != "X" && gameboard[index] != "O") {
        return true;
    }

    return false;
}