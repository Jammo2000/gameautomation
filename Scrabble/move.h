#ifndef MOVE_H
#define MOVE_H
#include <string>

#include "rules.h"
class Move {
   public:
    int x;
    int y;
    bool right;
    std::string word;

    int value;
    Move(int x, int y, bool right, std::string word);
    // returns the points gained if this move was placed onto board
    int getPoints(const char (&board)[BoardSize][BoardSize]);

    bool operator==(const Move &other);
};
#endif