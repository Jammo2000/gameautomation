#include "board.h"

#include <iostream>

#include "board.h"
#include "move.h"
#include "rules.h"
#include "color.h"
std::map<char, std::string> colors = {
	{EM, "Black"},
	{W2, "Light Magenta"},
	{W3, "Red"},
	{L2, "Light Blue"},
	{L3, "Blue"}
};
char Board::at(int x, int y) const { return data[y][x]; }
void Board::applyMove(const Move& move) {
    if (move.x + move.word.length() * move.right > size ||
        move.y + move.word.length() * !move.right > size) {
        throw std::out_of_range("Move does not fit on the board");
    }
    int x = move.x;
    int y = move.y;
    for (int i = 0; i < move.word.length(); i++) {
        data[y][x] = move.word[i];
        x += move.right;
        y += !move.right;
    }
}
void Board::print() const {
    std::cout << "  |";
    for (int x = 0; x < Board::size; x++) {
        std::cout << (x < 10 ? x : x - 10) << "|";
    }
    std::cout << std::endl;
    for (int y = 0; y < Board::size; y++) {
        std::cout << (y < 10 ? " " : "") << y << "|";
        for (int x = 0; x < Board::size; x++) {
            if (isLetter(data[y][x])) {
                std::cout << data[y][x];
            } else {
								std::cout<<color::rize(" ", "Black", colors.at(data[y][x]));
            }
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
Board::Board(const std::initializer_list<std::initializer_list<char> > args) {
    if (args.size() != size)
        throw std::invalid_argument("Must initialize Board to correct size");
    if (args.begin()->size() != size)
        throw std::invalid_argument("Must initialize Board to correct size");
    int y = 0;
    for (std::initializer_list<char> row : args) {
			  int x = 0;
        for (char c : row) {
            data[y][x] = c;
            x++;
        }
        y++;
    }
}
std::ostream& operator<<(std::ostream& os, const Board& board) {
    board.print();
    return os;
}