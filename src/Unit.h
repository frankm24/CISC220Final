#include "player.h"
#include <iostream>
using std::string;

class Unit {
    friend class Board;
    friend class Structure;
    bool isRevealed;
    string data;
    Structure *parent;
public:
    Unit();
    ~Unit();
    void reveal(); // show pointer in terminal and update reveal
}; 