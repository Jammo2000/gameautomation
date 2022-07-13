#ifndef MOVE_H
#define MOVE_H
#include <iostream>
#include <string>

#include "alphabetset.h"
struct Board;
class Move {
   public:
    int x;
    int y;
    bool right;
    std::string word;

    Move(int x, int y, bool right, std::string word);
    // returns the points gained if this move was placed onto board
    int getPoints(const Board& board) const;
    AlphabetSet tilesUsed(const Board& board) const;
    bool operator==(const Move& other) const;
};
std::ostream& operator<<(std::ostream& os, const Move& move);
#endif