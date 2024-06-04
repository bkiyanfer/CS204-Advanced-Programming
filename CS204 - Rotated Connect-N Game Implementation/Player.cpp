#include "Player.h"

using namespace std;

Player::Player(Board& board, char symbol) : board(board), symbol(symbol) {}

bool Player::put(int column) {
    return board.put_piece(symbol, column);
}

void Player::turn_board() {
    board.turn_board_clockwise();
}

bool Player::is_winner() const {
    char status = board.get_game_status();
    return status == symbol;
}

char Player::get_symbol() const {
    return symbol;
}
