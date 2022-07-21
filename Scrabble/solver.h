#ifndef SOLVER_H
#define SOLVER_H
#include <atomic>
#include <functional>
#include <initializer_list>
#include <vector>

#include "alphabetset.h"
#include "board.h"
#include "move.h"
std::vector<Move> getLegalMoves(
    const Board& board, const AlphabetSet& tilesAvailable, std::atomic<float>& completion);
extern Board board;
#endif  // SOLVER_H