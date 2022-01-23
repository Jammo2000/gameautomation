#ifndef RULES_H
#define RULES_H
#define BoardSize 15
#include <unordered_map>

#include "alphabetset.h"
const AlphabetSet tileValues{
    1,
    3,
    3,
    2,
    1,
    4,
    2,
    4,
    1,
    8,
    5,
    1,
    3,
    1,
    1,
    3,
    10,
    1,
    1,
    1,
    1,
    4,
    4,
    8,
    4,
    10,
};

const char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                           'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                           'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
enum specialSpace { EM,
                    L3,
                    L2,
                    W3,
                    W2 };

inline bool isLetter(char c) { return c >= 'A' && c <= 'Z'; }
#endif