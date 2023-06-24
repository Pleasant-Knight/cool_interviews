
/**
    Orginial problem connect four: https://neetcode.io/courses/ood-interview/0

Game Requirements: Some possible questions to ask:

    What are the rules of the game?
    What size is the grid?
    How many players are there? Player vs Computer? Player vs Player?
    Are we keeping track of the score?

Basics:
    The game will be played by only two players, player vs player
    The game board should be of variable dimensions
    The target is to connect N discs in a row (vertically, horizontally or diagonally)
    N is a variable (e.g. connect 4, 5, 6, etc)
    There should be a score tracking system
    After a player reaches the target score, they are the winner

Design High-level:
    We will need a Grid class to maintain the state of the 2-D board
    The board cell can be empty, yellow (occupied by Player 1) or red (occupied by Player 2)
    The grid will also be responsible for checking for a win condition
    We can have a Player class to represent the player's piece color
    This isn't super important, but encapsulating information is generally a good practice

    The Game class will be composed of the Grid and Players
    The Game class will be responsible for the game loop and keeping track of the score

*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Color of the tokens in the grid.
enum GridPosition {
    EMPTY, YELLOW, RED
};


// The grid class.
class Grid {
private:
    int rows;
    int columns;
    vector<vector<int>> grid;

public:
    Grid(int rows, int columns) {
        this->rows = rows;
        this->columns = columns;
        initGrid();
    }

    void initGrid() {
        this->grid = vector<vector<int>>();
        for (int i = 0; i < rows; i++) {
            this->grid.push_back(vector<int>());
            for (int j = 0; j < columns; j++) {
                grid[i].push_back(GridPosition::EMPTY);
            }
        }
    }

    vector<vector<int>> getGrid() {
        return this->grid;
    }

    int getColumnCount() {
        return this->columns;
    }

    int placePiece(int column, GridPosition piece) {
        if (column < 0 || column >= this->columns) {
            throw "Invalid column";
        }
        if (piece == GridPosition::EMPTY) {
            throw "Invalid piece";
        }
        // Place piece in the lowest empty row alawys.
        for (int row = this->rows - 1; row >= 0; row--) {
            if (this->grid[row][column] == GridPosition::EMPTY) {
                this->grid[row][column] = piece;
                return row;
            }
        }
        return -1;
    }

    bool checkWin(int connectN, int row, int col, GridPosition piece) {
        // Check horizontal connect-four.
        int count = 0;
        for (int c = 0; c < this->columns; c++) {
            if (this->grid[row][c] == piece) {
                count++;
            } else {
                count = 0;
            }
            if (count == connectN) {
                cout << "Won horizontal\n";
                return true;
            }
        }

        // Check vertical connect-four.
        count = 0;
        for (int r = 0; r < this->rows; r++) {
            if (this->grid[r][col] == piece) {
                count++;
            } else {
                count = 0;
            }
            if (count == connectN) {
                cout << "Won vertical\n";
                return true;
            }
        }

        // Check diagonal connect-four.
        count = 0;
        for (int r = 0; r < this->rows; r++) {
            int c = row + col - r; // row + col = r + c, for a diagonal
            if (c >= 0 && c < this->columns && this->grid[r][c] == piece) {
                count++;
            } else {
                count = 0;
            }
            if (count == connectN) {
                cout << "Won diagonal\n";
                return true;
            }
        }

        // Check anti-diagonal connect-four.
        count = 0;
        for (int r = 0; r < this->rows; r++) {
            int c = col - row + r; // row - col = r - c, for an anti-diagonal
            if (c >= 0 && c < this->columns && this->grid[r][c] == piece) {
                count++;
            } else {
                count = 0;
            }
            if (count == connectN) {
                cout << "Won anti-diagonal\n";
                return true;
            }
        }
        return false;
    }
};

// The player class.
class Player {
private:
    string name;
    GridPosition piece;

public:
    Player(string name, GridPosition piece) {
        this->name = name;
        this->piece = piece;
    }

    string getName() {
        return this->name;
    }

    GridPosition getPieceColor() {
        return this->piece;
    }
};


class Game {
private:
    Grid* grid;
    int connectN; 						// Connect four or more.
    vector<Player*> players; 			// Moore than two players.
    unordered_map<string, int> score; 	// The score board.
    int targetScore;					// How many rounds per game.
    int colCnt;                         // How many columns per grid.

public:
    Game(Grid* grid, int connectN, int targetScore) {
        this->grid = grid;
        this->connectN = connectN;
        this->targetScore = targetScore;
        this->colCnt = grid->getColumnCount();

        // Could use unique_ppointer.
        this->players = vector<Player*> {
            new Player("Player A", YELLOW),
            new Player("Player B", RED)
        };

        this->score = unordered_map<string, int>();
        for (Player* player : this->players) {
            this->score[player->getName()] = 0;
        }
    }

    void printBoard() {
        cout << "Board:" << "\n";
        vector<vector<int>> grid = this->grid->getGrid();
        for (size_t i = 0; i < grid.size(); i++) {
            string row = "";
            for (auto piece : grid[i]) {
                if (piece == GridPosition::EMPTY) {
                    row += "0 ";
                } else if (piece == GridPosition::YELLOW) {
                    row += "Y ";
                } else if (piece == GridPosition::RED) {
                    row += "R ";
                }
            }
            cout << row << "\n";
        }
        cout << "\n";
    }

    vector<int> playMove(Player* player) {
        printBoard();
        cout << player->getName() << "'s turn" << "\n";
        //int colCnt = this->grid->getColumnCount();

        cout << "Enter column between 0 and " << (colCnt - 1) << " to add piece: ";
        int moveColumn = 0;
        cin >> moveColumn;

        int moveRow = this->grid->placePiece(moveColumn, player->getPieceColor());
        return vector<int> { moveRow, moveColumn };
    }

    Player* playRound() {
        // Play till the round is done.
        while (true) {
            for (Player* player : this->players) {
                vector<int> pos = playMove(player);
                int row = pos[0];
                int col = pos[1];

                GridPosition pieceColor = player->getPieceColor();
                if (this->grid->checkWin(this->connectN, row, col, pieceColor)) {
                    this->score[player->getName()] = this->score[player->getName()] + 1;
                    return player;
                }
            }
        }
    }

    void play() {
        int maxScore = 0;
        Player* winner = nullptr;
        while (maxScore < this->targetScore) {
            winner = playRound();
            cout << "Player: " << winner->getName() << " won the round" << "\n";
            maxScore = max(this->score[winner->getName()], maxScore);

            this->grid->initGrid(); // reset grid
        }
        cout << winner->getName() << " won the game" << "\n";
    }
};

int main() {
    Grid* grid = new Grid(6, 7);
    Game* game = new Game(grid, 4, 3);
    game->play();

    return 0;
}

