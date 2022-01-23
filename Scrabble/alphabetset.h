#ifndef ALPHABETSET_H
#define ALPHABETSET_H
#include <stdint.h>
#include <stdio.h>

#include <initializer_list>
#include <string>

#include "rules.h"
class AlphabetSet {
   protected:
    unsigned short data[26];
    inline unsigned short charToIndex(char c);

   public:
    void insert(char c);

    void insert(AlphabetSet &other);
    void insert(std::string &s);
    unsigned short at(char c) const;
    bool contains(char c) const;

    bool contains(AlphabetSet &other) const;
    void remove(char c);
    void remove(AlphabetSet &other);
    void reset();
    void print() const;
    AlphabetSet();
    AlphabetSet(std::initializer_list<char> list);
};
#endif