#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    char** board;
    int size;
    int win_length;

    bool check_winner(char symbol, int startRow, int startCol) const;
    void drop_pieces();

public:
    Board(int size, int win_length);
    ~Board();

    void print_board() const;
    bool put_piece(char symbol, int column);
    void turn_board_clockwise();
    char get_game_status() const;
};

#endif // BOARD_H
