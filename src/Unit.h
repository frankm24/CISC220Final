#include "player.h"
#include <iostream>
using std::string;

class Unit {
    friend class Board;
    bool is_revealed;
    string data;
    Structure *parent;
public:
    Unit();
    ~Unit();
    void reveal(); // show pointer in terminal and update reveal
}; 