#include "Player.h"
#include <iostream>

#ifndef CISC220FINAL_UNIT_H
#define CISC220FINAL_UNIT_H
using std::string;

class Unit {
    friend class Board;
    friend class Structure;
    bool isRevealed;
    string data;
    Structure *parent;
public:
    Unit();
    Unit(string data, Structure *s, bool revealed );
    ~Unit();
    void reveal(); // show pointer in terminal and update reveal
};

#endif //CISC220FINAL_UNIT_H
