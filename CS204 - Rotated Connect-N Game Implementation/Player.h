#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"

class Player {
private:
    Board& board;
    char symbol;

public:
    Player(Board& board, char symbol);

    bool put(int column);
    void turn_board();
    bool is_winner() const;
    char get_symbol() const;
};

#endif // PLAYER_H
