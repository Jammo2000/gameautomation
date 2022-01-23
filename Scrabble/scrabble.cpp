#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>

#include "alphabetset.h"
#include "move.h"
#include "rules.h"
using namespace std;

char board[BoardSize][BoardSize] = {
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

void printBoard(const char (&board)[BoardSize][BoardSize]) {
    cout << "    |";
    for (int x = 0; x < BoardSize; x++) {
        cout << (x < 10 ? x : x - 10) << "|";
    }
    cout << endl;
    for (int y = 0; y < BoardSize; y++) {
        cout << (y < 10 ? " " : "") << y << "|";
        for (int x = 0; x < BoardSize; x++) {
            if (isLetter(board[y][x])) {
                cout << board[y][x];
            } else {
                cout << " ";
            }
            cout << "|";
        }
        cout << endl;
    }
    cout << endl;
}

void makeMove(Move &move, char (&board)[BoardSize][BoardSize]) {
    int x = move.x;
    int y = move.y;
    for (int i = 0; i < move.word.length(); i++) {
        board[y][x] = move.word[i];
        x += move.right;
        y += !move.right;
    }
}

// which letters could be placed such that no invalid words would be
// created, if the main word is in the direction of parameter right.
array<array<AlphabetSet, BoardSize>, BoardSize> getAllowedLetters(
    const char (&board)[BoardSize][BoardSize], bool right) {
    array<array<AlphabetSet, BoardSize>, BoardSize> returnValue;
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (isLetter(board[y][x])) {
                returnValue[y][x].insert(board[y][x]);
            } else if (right) {
                if (x == 7 && y == 0) {
                    cout << "yeetus" << endl;
                }
                string crossWord = "";
                int searchY = y - 1;
                int letterPosition;  // position of letter in the word
                // search to tops
                while (searchY > 0 && isLetter(board[searchY][x])) {
                    searchY--;
                }
                if (searchY > 0 || y == 0) {
                    searchY++;
                }
                letterPosition = y - searchY;
                while (searchY < BoardSize &&
                       (isLetter(board[searchY][x]) || searchY == y)) {
                    crossWord += board[searchY][x];
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
                while (searchX > 0 && isLetter(board[y][searchX])) {
                    searchX--;
                }
                if (searchX > 0 || x == 0) {
                    searchX++;
                }
                letterPosition = x - searchX;
                while (searchX < BoardSize &&
                       (isLetter(board[y][searchX]) || searchX == x)) {
                    crossWord += board[y][searchX];
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

vector<Move> getLegalMoves(const char (&board)[BoardSize][BoardSize],
                           AlphabetSet tilesAvailable) {
    vector<Move> moves;
    bool hasCharacter = false;
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (isLetter(board[y][x])) {
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
                float completion = (float)(wordIndex + wordlist.size() * right) /
                                   (wordlist.size() * 2);
                if (floor(completion * 100) > floor(prevCompletion * 100)) {
                    cout << "finding move, " << floor(completion * 100) << "% complete"
                         << endl;
                    prevCompletion = completion;
                }
                string word = wordlist[wordIndex];
                int wordLength = word.length();
                int maxX = right ? BoardSize - wordLength : BoardSize - 1;
                int maxY = right ? BoardSize - 1 : BoardSize - wordLength;
                for (int x = 0; x < maxX; x++) {
                    for (int y = 0; y < maxY; y++) {
                        AlphabetSet tilesUsed;
                        bool hasNeighbors = false;
                        bool placesTiles = false;
                        if (right) {
                            if (x > 0 && isLetter(board[y][x - 1])) {
                                continue;
                            }
                            if (x < BoardSize && isLetter(board[y][x + wordLength])) {
                                continue;
                            }
                            for (int i = 0; i < wordLength; i++) {
                                if (!isLetter(board[y][x + i])) {
                                    if (!validLetters[y][x + i].contains(word[i])) {
                                        goto nextWord;
                                    }
                                    placesTiles = true;
                                    tilesUsed.insert(word[i]);
                                } else {
                                    if (board[y][x + i] != word[i]) {
                                        goto nextWord;
                                    }
                                    // crosses another word
                                    hasNeighbors = true;
                                }
                                if (!hasNeighbors) {
                                    // touches on side
                                    if ((y > 0 && isLetter(board[y - 1][x + i])) ||
                                        (y < BoardSize - 1 && isLetter(board[y + 1][x + i]))) {
                                        hasNeighbors = true;
                                    }
                                }
                            }
                        } else {
                            if (y > 0 && isLetter(board[y - 1][x])) {
                                continue;
                            }
                            if (y < BoardSize && isLetter(board[y + wordLength][x])) {
                                continue;
                            }
                            for (int i = 0; i < wordLength; i++) {
                                if (!isLetter(board[y + i][x])) {
                                    if (!validLetters[y + i][x].contains(word[i])) {
                                        goto nextWord;
                                    }
                                    placesTiles = true;
                                    tilesUsed.insert(word[i]);
                                } else {
                                    if (board[y + i][x] != word[i]) {
                                        goto nextWord;
                                    }
                                    // crosses another word
                                    hasNeighbors = true;
                                }
                                if (!hasNeighbors) {
                                    // touches on side
                                    if ((x > 0 && isLetter(board[y + i][x - 1])) ||
                                        (x < BoardSize - 1 && isLetter(board[y + i][x + 1]))) {
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
            if (word == "LINE") {
                cout << "line" << endl;
            }
            AlphabetSet tilesUsed;
            for (char c : word) {
                tilesUsed.insert(c);
            }
            if (!tilesAvailable.contains(tilesUsed)) {
                continue;
            }
            for (int x = 0; x < BoardSize; x++) {
                for (int y = 0; y < BoardSize; y++) {
                    for (bool right : dirs) {
                        int endX = x + (word.length() - 1) * right;
                        int endY = y + (word.length() - 1) * !right;
                        if (endX >= BoardSize || endY >= BoardSize) {
                            continue;
                        }
                        int center = BoardSize / 2;
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

void toUpperCase(string &s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return toupper(c); });
}

void updateTileSet(AlphabetSet &tileSet) {
    string input;
    cout << "Enter the drawn tiles, then press enter: ";
    cin >> input;
    toUpperCase(input);
    cout << input;
    tileSet.insert(input);
}

int main() {
    cout << wordlist.size() << " words loaded to dictionary" << endl;
    AlphabetSet tiles;
    while (true) {
        updateTileSet(tiles);
        cout << "tiles: ";
        tiles.print();
        vector<Move> moves = getLegalMoves(board, tiles);
        cout << moves.size() << " legal moves found" << endl;
        cout << "evaluating moves" << endl;
        for (size_t i = 0; i < moves.size(); i++) {
            moves[i].value = moves[i].getPoints(board);
        }
        cout << "sorting moves by value" << endl;
        sort(moves.begin(), moves.end(),
             [](const Move &a, const Move &b) { return b.value < a.value; });
        int moveChoice = 0;
        string input = "0";
        do {
            moveChoice = stoi(input);
            char newBoard[BoardSize][BoardSize];
            memcpy(newBoard, board, BoardSize * BoardSize * sizeof(char));
            makeMove(moves[moveChoice], newBoard);
            int points = moves[moveChoice].getPoints(board);
            cout << "Move #" << moveChoice << ", for " << points
                 << " points:" << endl;
            printBoard(newBoard);
            cout << "Enter new move #, or s to select: ";
            cin >> input;
        } while (input != "s");
        makeMove(moves[moveChoice], board);
        cout << "Move made:" << endl;
        printBoard(board);
    }
    cout << "done";
}