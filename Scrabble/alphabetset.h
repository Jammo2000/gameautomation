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
    unsigned short operator[](char c);
    bool contains(char c);

    bool contains(AlphabetSet &other);
    void remove(char c);
    void remove(AlphabetSet &other);
    void reset();
    void print();
    AlphabetSet();
    AlphabetSet(std::initializer_list<char> list);
};
#endif