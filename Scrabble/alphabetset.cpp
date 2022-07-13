#include "alphabetset.h"

#include <iostream>
#include <stdexcept>
#include <cstring>
#include "rules.h"

using namespace std;

unsigned short AlphabetSet::charToIndex(char c) { return c - 'A'; }

void AlphabetSet::insert(char c) { data[charToIndex(c)]++; }

void AlphabetSet::insert(const AlphabetSet &other) {
    for (uint8_t i = 0; i < 26; i++) {
        data[i] += other.data[i];
    }
}
void AlphabetSet::insert(string &s) {
    for (char c : s) {
        insert(c);
    }
}
void AlphabetSet::setQuantity(char c, unsigned short quantity) {
    if (isLetter(c)) {
        data[charToIndex(c)] = quantity;
    } else {
        throw out_of_range("Character is not a letter");
    }
}

void AlphabetSet::setDataDirect(unsigned short newData[26]) {
    memcpy(newData, data, sizeof(data));
}

unsigned short AlphabetSet::at(char c) const {
    if (isLetter(c)) {
        return data[charToIndex(c)];
    } else {
        throw out_of_range("Character is not a letter");
    }
}

bool AlphabetSet::contains(char c) const { return data[charToIndex(c)] > 0; }

bool AlphabetSet::contains(const AlphabetSet &other) const {
    for (uint8_t i = 0; i < 26; i++) {
        if (data[i] < other.data[i]) {
            return false;
        }
    }
    return true;
}

unsigned AlphabetSet::total() const{
	unsigned result = 0;
	for (uint8_t i = 0; i < 26; i++) {
        result+=data[i];
  }
	return result;
}
void AlphabetSet::remove(char c) {
    if (data[charToIndex(c) > 0]) {
        data[charToIndex(c)]--;
    } else {
        throw invalid_argument("Alphabet Sets may not contain negatives");
    }
}
void AlphabetSet::remove(const AlphabetSet &other) {
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
void AlphabetSet::print() const {
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
AlphabetSet::AlphabetSet(bool isLiteralData, initializer_list<unsigned short> list) {
    for (uint8_t i = 0; i < 26; i++) {
        data[i] = (unsigned short)list.begin()[i];
    }
}