#include "move.h"

#include <string>

#include "rules.h"

using namespace std;
Move::Move(int x, int y, bool right, string word) {
    this->x = x;
    this->y = y;
    this->right = right;
    this->word = word;
    value = 0;
}
// returns the points gained if this move was placed onto board
// assumes the move is legal
int Move::getPoints(const char (&board)[BoardSize][BoardSize]) {
    int mainPoints = 0;
    int mainMultiplier = 1;
    int tilesPlaced = 0;
    int totalCrossPoints = 0;
    if (right) {
        for (int i = 0; i < word.length(); i++) {
            tilesPlaced++;
            switch (board[y][x + i]) {
                case L3:
                    mainPoints += 3 * tileValues.at(word[i]);
                    break;
                case L2:
                    mainPoints += 2 * tileValues.at(word[i]);
                    break;
                case W2:
                    mainPoints += tileValues.at(word[i]);
                    mainMultiplier *= 2;
                    break;
                case W3:
                    mainPoints += tileValues.at(word[i]);
                    mainMultiplier *= 3;
                    break;
                case EM:
                    mainPoints += tileValues.at(word[i]);
                    break;
                default:
                    mainPoints += tileValues.at(word[i]);
                    tilesPlaced--;
            }
            if (!isLetter(board[y][x + i])) {
                // cross words
                int letterPosition;
                int searchTop;
                string crossWord = "";
                int searchY = y - 1;
                // search to tops
                while (searchY > 0 && isLetter(board[searchY][x + i])) {
                    searchY--;
                }
                if (searchY > 0) {
                    searchY++;
                }
                searchTop = searchY;
                letterPosition = y - searchY;
                while (searchY < BoardSize &&
                       (isLetter(board[searchY][x + i] || searchY == y))) {
                    crossWord += board[searchY][x + i];
                    searchY++;
                }
                if (crossWord.length() > 1 && !isLetter(crossWord[letterPosition])) {
                    int crossPoints = 0;
                    int crossMultiplier = 1;
                    crossWord[letterPosition] = word[i];
                    for (int j = 0; j < crossWord.length(); j++) {
                        switch (board[searchTop + j][x + i]) {
                            case L3:
                                crossPoints += 3 * tileValues.at(crossWord[j]);
                                break;
                            case L2:
                                crossPoints += 2 * tileValues.at(crossWord[j]);
                                break;
                            case W2:
                                crossPoints += tileValues.at(crossWord[j]);
                                crossMultiplier *= 2;
                                break;
                            case W3:
                                crossPoints += tileValues.at(crossWord[j]);
                                crossMultiplier *= 3;
                                break;
                            default:
                                crossPoints += tileValues.at(crossWord[j]);
                        }
                        totalCrossPoints += crossPoints * crossMultiplier;
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < word.length(); i++) {
            tilesPlaced++;
            switch (board[y + i][x]) {
                case L3:
                    mainPoints += 3 * tileValues.at(word[i]);
                    break;
                case L2:
                    mainPoints += 2 * tileValues.at(word[i]);
                    break;
                case W2:
                    mainPoints += tileValues.at(word[i]);
                    mainMultiplier *= 2;
                    break;
                case W3:
                    mainPoints += tileValues.at(word[i]);
                    mainMultiplier *= 3;
                    break;
                case EM:
                    mainPoints += tileValues.at(word[i]);
                    tilesPlaced++;
                    break;
                default:
                    mainPoints += tileValues.at(word[i]);
                    tilesPlaced--;
            }
            if (!isLetter(board[y + i][x])) {
                // cross words
                int letterPosition;
                string crossWord = "";
                int searchX = x - 1;
                // search to tops
                while (searchX > 0 && isLetter(board[y][searchX])) {
                    searchX--;
                }
                if (searchX > 0) {
                    searchX++;
                }
                int searchTop = searchX;
                letterPosition = x - searchX;
                while (searchX < BoardSize &&
                       (isLetter(board[y][searchX]) || searchX == x)) {
                    crossWord += board[y][searchX];
                    searchX++;
                }
                if (crossWord.length() > 1 && !isLetter(crossWord[letterPosition])) {
                    int crossPoints = 0;
                    int crossMultiplier = 1;
                    crossWord[letterPosition] = word[i];
                    for (int j = 0; j < crossWord.length(); j++) {
                        switch (board[y + i][searchTop + j]) {
                            case L3:
                                crossPoints += 3 * tileValues.at(crossWord[j]);
                                break;
                            case L2:
                                crossPoints += 2 * tileValues.at(crossWord[j]);
                                break;
                            case W2:
                                crossPoints += tileValues.at(crossWord[j]);
                                crossMultiplier *= 2;
                                break;
                            case W3:
                                crossPoints += tileValues.at(crossWord[j]);
                                crossMultiplier *= 3;
                                break;
                            default:
                                crossPoints += tileValues.at(crossWord[j]);
                        }
                        totalCrossPoints += crossPoints * crossMultiplier;
                    }
                }
            }
        }
    }
    return mainPoints * mainMultiplier + totalCrossPoints +
           (tilesPlaced >= 7 ? 50 : 0);
}

bool Move::operator==(const Move &other) {
    return x == other.x && y == other.y && right == other.right &&
           word == other.word;
}