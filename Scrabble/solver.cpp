#include "solver.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <vector>

#include "alphabetset.h"
#include "board.h"
#include "move.h"
#include "rules.h"
#include "solver.h"
using namespace std;

Board board = {
    {W3, EM, EM, L2, EM, EM, EM, W3, EM, EM, EM, L2, EM, EM, W3},
    {EM, W2, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, W2, EM},
    {EM, EM, W2, EM, EM, EM, L2, EM, L2, EM, EM, EM, W2, EM, EM},
    {L2, EM, EM, W2, EM, EM, EM, L2, EM, EM, EM, W2, EM, EM, L2},
    {EM, EM, EM, EM, W2, EM, EM, EM, EM, EM, W2, EM, EM, EM, EM},
    {EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM},
    {EM, EM, L2, EM, EM, EM, L2, EM, L2, EM, EM, EM, L2, EM, EM},
    {W3, EM, EM, L2, EM, EM, EM, W2, EM, EM, EM, L2, EM, EM, W3},
    {EM, EM, L2, EM, EM, EM, L2, EM, L2, EM, EM, EM, L2, EM, EM},
    {EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM},
    {EM, EM, EM, EM, W2, EM, EM, EM, EM, EM, W2, EM, EM, EM, EM},
    {L2, EM, EM, W2, EM, EM, EM, L2, EM, EM, EM, W2, EM, EM, L2},
    {EM, EM, W2, EM, EM, EM, L2, EM, L2, EM, EM, EM, W2, EM, EM},
    {EM, W2, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, W2, EM},
    {W3, EM, EM, L2, EM, EM, EM, W3, EM, EM, EM, L2, EM, EM, W3},
};
vector<string> readWordList(string filename) {
    fstream file;
    string word;
    vector<string> words;
    // opening file
    file.open(filename.c_str());
    // extracting words from the file
    while (file >> word) {
        words.push_back(word);
    }
    words.shrink_to_fit();
    return words;
}

const vector<string> wordlist = readWordList("wordlist.txt");

int maxTiles;
bool isInWordlist(string word) {
    int min = 0;
    int max = wordlist.size() - 1;
    while (min <= max) {
        int middle = (min + max) / 2;
        if (wordlist[middle] < word) {
            min = middle + 1;
        } else if (wordlist[middle] > word) {
            max = middle - 1;
        } else {
            return true;
        }
    }
    return false;
}

// which letters could be placed such that no invalid words would be
// created, if the main word is in the direction of parameter right.
array<array<AlphabetSet, Board::size>, Board::size> getAllowedLetters(
    const Board &board, bool right) {
    array<array<AlphabetSet, Board::size>, Board::size> returnValue;
    for (int x = 0; x < Board::size; x++) {
        for (int y = 0; y < Board::size; y++) {
            if (isLetter(board.at(x, y))) {
                returnValue[y][x].insert(board.at(x, y));
            } else if (right) {
                if (x == 7 && y == 0) {
                    cout << "yeetus" << endl;
                }
                string crossWord = "";
                int searchY = y - 1;
                int letterPosition;  // position of letter in the word
                // search to tops
                while (searchY > 0 && isLetter(board.at(x, searchY))) {
                    searchY--;
                }
                if (searchY > 0 || y == 0) {
                    searchY++;
                }
                letterPosition = y - searchY;
                while (searchY < Board::size &&
                       (isLetter(board.at(x, searchY)) || searchY == y)) {
                    crossWord += board.at(x, searchY);
                    searchY++;
                }
                if (crossWord.length() > 1) {
                    for (char letter : alphabet) {
                        crossWord[letterPosition] = letter;
                        if (isInWordlist(crossWord)) {
                            returnValue[y][x].insert(letter);
                        }
                    }
                } else {
                    for (char letter : alphabet) {
                        returnValue[y][x].insert(letter);
                    }
                }
            } else {
                string crossWord = "";
                int searchX = x - 1;
                int letterPosition;  // position of letter in the word
                // search to tops
                while (searchX > 0 && isLetter(board.at(searchX, y))) {
                    searchX--;
                }
                if (searchX > 0 || x == 0) {
                    searchX++;
                }
                letterPosition = x - searchX;
                while (searchX < Board::size &&
                       (isLetter(board.at(searchX, y)) || searchX == x)) {
                    crossWord += board.at(searchX, y);
                    searchX++;
                }
                if (crossWord.length() > 1) {
                    for (char letter : alphabet) {
                        crossWord[letterPosition] = letter;
                        if (isInWordlist(crossWord)) {
                            returnValue[y][x].insert(letter);
                        }
                    }
                } else {
                    for (char letter : alphabet) {
                        returnValue[y][x].insert(letter);
                    }
                }
            }
        }
    }
    return returnValue;
}

vector<Move> getLegalMoves(const Board &board, const AlphabetSet &tilesAvailable) {
    vector<Move> moves;
    bool hasCharacter = false;
    for (int x = 0; x < Board::size; x++) {
        for (int y = 0; y < Board::size; y++) {
            if (isLetter(board.at(x, y))) {
                hasCharacter = true;
            }
        }
    }

    bool dirs[] = {false, true};
    float prevCompletion = 0;
    if (hasCharacter) {
        for (bool right : dirs) {
            auto validLetters = getAllowedLetters(board, right);
            for (int wordIndex = 0; wordIndex < wordlist.size(); wordIndex++) {
                float completion =
                    (float)(wordIndex + wordlist.size() * right) /
                    (wordlist.size() * 2);
                if (floor(completion * 100) > floor(prevCompletion * 100)) {
                    cout << "finding move, " << floor(completion * 100)
                         << "% complete" << endl;
                    prevCompletion = completion;
                }
                string word = wordlist[wordIndex];
                int wordLength = word.length();
                int maxX = right ? Board::size - wordLength : Board::size - 1;
                int maxY = right ? Board::size - 1 : Board::size - wordLength;
                for (int x = 0; x < maxX; x++) {
                    for (int y = 0; y < maxY; y++) {
                        AlphabetSet tilesUsed;
                        bool hasNeighbors = false;
                        bool placesTiles = false;
                        if (right) {
                            if (x > 0 && isLetter(board.at(x - 1, y))) {
                                continue;
                            }
                            if (x < Board::size &&
                                isLetter(board.at(x + wordLength, y))) {
                                continue;
                            }
                            for (int i = 0; i < wordLength; i++) {
                                if (!isLetter(board.at(x + i, y))) {
                                    if (!validLetters[y][x + i].contains(
                                            word[i])) {
                                        goto nextWord;
                                    }
                                    placesTiles = true;
                                    tilesUsed.insert(word[i]);
                                } else {
                                    if (board.at(x + i, y) != word[i]) {
                                        goto nextWord;
                                    }
                                    // crosses another word
                                    hasNeighbors = true;
                                }
                                if (!hasNeighbors) {
                                    // touches on side
                                    if ((y > 0 &&
                                         isLetter(board.at(x + i, y - 1))) ||
                                        (y < Board::size - 1 &&
                                         isLetter(board.at(x + i, y + 1)))) {
                                        hasNeighbors = true;
                                    }
                                }
                            }
                        } else {
                            if (y > 0 && isLetter(board.at(x, y - 1))) {
                                continue;
                            }
                            if (y < Board::size &&
                                isLetter(board.at(x, y + wordLength))) {
                                continue;
                            }
                            for (int i = 0; i < wordLength; i++) {
                                if (!isLetter(board.at(x, y + i))) {
                                    if (!validLetters[y + i][x].contains(
                                            word[i])) {
                                        goto nextWord;
                                    }
                                    placesTiles = true;
                                    tilesUsed.insert(word[i]);
                                } else {
                                    if (board.at(x, y + i) != word[i]) {
                                        goto nextWord;
                                    }
                                    // crosses another word
                                    hasNeighbors = true;
                                }
                                if (!hasNeighbors) {
                                    // touches on side
                                    if ((x > 0 &&
                                         isLetter(board.at(x - 1, y + i))) ||
                                        (x < Board::size - 1 &&
                                         isLetter(board.at(x + 1, y + i)))) {
                                        hasNeighbors = true;
                                    }
                                }
                            }
                        }
                        if (hasNeighbors && placesTiles &&
                            tilesAvailable.contains(tilesUsed)) {
                            Move move(x, y, right, word);
                            moves.push_back(move);
                        }
                    nextWord:;
                    }
                }
            }
        }
    } else {  // First move
        for (string word : wordlist) {
            if (word.length() < 3) {
                continue;  // rule nobody remembers
            }
            AlphabetSet tilesUsed;
            for (char c : word) {
                tilesUsed.insert(c);
            }
            if (!tilesAvailable.contains(tilesUsed)) {
                continue;
            }
            for (int x = 0; x < Board::size; x++) {
                for (int y = 0; y < Board::size; y++) {
                    for (bool right : dirs) {
                        int endX = x + (word.length() - 1) * right;
                        int endY = y + (word.length() - 1) * !right;
                        if (endX >= Board::size || endY >= Board::size) {
                            continue;
                        }
                        int center = Board::size / 2;
                        if (x == center) {
                            if (y <= center && endY >= center) {
                                Move m(x, y, right, word);

                                moves.push_back(m);
                            }
                        } else if (y == center) {
                            if (x <= center && endX >= center) {
                                Move m(x, y, right, word);
                                moves.push_back(m);
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}
