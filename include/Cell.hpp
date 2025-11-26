#ifndef CISC220FINAL_UNIT_H
#define CISC220FINAL_UNIT_H

#include <iostream>

using std::string;

class Structure;
class Board;

class Cell {
    friend class Board;
    friend class Structure;
    bool isRevealed;
    string data;
    Structure *parent;
public:
    Cell();
    Cell(string data, Structure *s, bool revealed );
    ~Cell();
    void reveal(); // show pointer in terminal and update reveal
};

#endif //CISC220FINAL_UNIT_H
