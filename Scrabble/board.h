#ifndef BOARD_H
#define BOARD_H
#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "move.h"
struct Board {
    static const int size = 15;
    void applyMove(const Move &move);
    void print() const;
    char data[size][size];
    char at(int x, int y) const;
    Board(const std::initializer_list<std::initializer_list<char> > args);
};
enum specialSpace { EM, L3, L2, W3, W2 };
#endif