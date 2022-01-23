#include "alphabetset.h"

#include <iostream>
#include <stdexcept>

using namespace std;

unsigned short AlphabetSet::charToIndex(char c) { return c - 'A'; }

void AlphabetSet::insert(char c) { data[charToIndex(c)]++; }

void AlphabetSet::insert(AlphabetSet &other) {
    for (uint8_t i = 0; i < 26; i++) {
        data[i] += other.data[i];
    }
}
void AlphabetSet::insert(string &s) {
    for (char c : s) {
        insert(c);
    }
}
unsigned short AlphabetSet::operator[](char c) {
    if (isLetter(c)) {
        return data[charToIndex(c)];
    } else {
        throw out_of_range("Character is not a letter");
    }
}

bool AlphabetSet::contains(char c) { return data[charToIndex(c)] > 0; }

bool AlphabetSet::contains(AlphabetSet &other) {
    for (uint8_t i = 0; i < 26; i++) {
        if (data[i] < other.data[i]) {
            return false;
        }
    }
    return true;
}
void AlphabetSet::remove(char c) {
    if (data[charToIndex(c) > 0]) {
        data[charToIndex(c)]--;
    } else {
        throw invalid_argument("Alphabet Sets may not contain negatives");
    }
}
void AlphabetSet::remove(AlphabetSet &other) {
    for (uint8_t i = 0; i < 26; i++) {
        if (data[i] >= other.data[i]) {
            data[i] -= other.data[i];
        } else {
            throw invalid_argument("Alphabet Sets may not contain negatives");
        }
    }
}
void AlphabetSet::reset() {
    for (uint8_t i = 0; i < 26; i++) {
        data[i] = 0;
    }
}
void AlphabetSet::print() {
    for (uint8_t i = 0; i < 26; i++) {
        cout << data[i] << " ";
    }
}
AlphabetSet::AlphabetSet() { reset(); }
AlphabetSet::AlphabetSet(initializer_list<char> list) {
    reset();
    for (char c : list) {
        insert(c);
    }
}