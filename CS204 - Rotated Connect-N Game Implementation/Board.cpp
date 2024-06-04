
//Beril Kiyanfer
//30565


#include "Board.h"
#include <iostream>
#include <algorithm>

using namespace std;

Board::Board(int size, int win_length) : size(size), win_length(win_length) {
    board = new char*[size];
    for (int i = 0; i < size; ++i) {
        board[i] = new char[size];
        fill(board[i], board[i] + size, '-');
    }
}

Board::~Board() {
    for (int i = 0; i < size; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

void Board::print_board() const {
    cout << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

bool Board::put_piece(char symbol, int column) {
    if (column < 0 || column >= size) return false;
    for (int i = size - 1; i >= 0; --i) {
        if (board[i][column] == '-') {
            board[i][column] = symbol;
            return true;
        }
    }
    return false;
}

void Board::turn_board_clockwise() {
    char** temp = new char*[size];
    for (int i = 0; i < size; i++) {
        temp[i] = new char[size];
    }

    // Rotate the board 90 degrees clockwise
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            temp[j][size - 1 - i] = board[i][j];
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = temp[i][j];
        }
        delete[] temp[i];
    }
    delete[] temp;

    drop_pieces();
}

void Board::drop_pieces() {
    for (int col = 0; col < size; col++) {
        int writeRow = size - 1;
        for (int row = size - 1; row >= 0; row--) {
            if (board[row][col] != '-') {
                board[writeRow--][col] = board[row][col];
                if (writeRow != row - 1) {
                    board[row][col] = '-';
                }
            }
        }
        while (writeRow >= 0) {
            board[writeRow--][col] = '-';
        }
    }
}

char Board::get_game_status() const {
    bool emptyFound = false;
    bool winnerFound[2] = {false, false}; // [0] -> 'O', [1] -> 'X'

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (board[row][col] == '-') {
                emptyFound = true;
            } else if (check_winner(board[row][col], row, col)) {
                if (board[row][col] == 'O') {
                    winnerFound[0] = true;
                } else if (board[row][col] == 'X') {
                    winnerFound[1] = true;
                }
            }
        }
    }

    if (winnerFound[0] && winnerFound[1]) {
        return 'D'; // Both players win then it is draw
    }
    if (winnerFound[0]) {
        return 'O';
    }
    if (winnerFound[1]) {
        return 'X';
    }
    return emptyFound ? '-' : 'D'; // If no empty spots then it's a draw tooo
}

bool Board::check_winner(char symbol, int startRow, int startCol) const {
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (auto& dir : directions) {
        int count = 1;
        for (int i = 1; i < win_length; ++i) {
            int newRow = startRow + dir[0] * i;
            int newCol = startCol + dir[1] * i;
            if (newRow < 0 || newRow >= size || newCol < 0 || newCol >= size || board[newRow][newCol] != symbol) {
                break;
            }
            count++;
        }
        for (int i = 1; i < win_length; ++i) {
            int newRow = startRow - dir[0] * i;
            int newCol = startCol - dir[1] * i;
            if (newRow < 0 || newRow >= size || newCol < 0 || newCol >= size || board[newRow][newCol] != symbol) {
                break;
            }
            count++;
        }
        if (count >= win_length) {
            return true;
        }
    }
    return false;
}
