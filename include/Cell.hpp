#ifndef CISC220FINAL_UNIT_H
#define CISC220FINAL_UNIT_H

#include <iostream>
#include <vector>

using std::string;
using std::vector;

class Structure;
class Board;

class Cell {
    friend class Board;
    friend class Structure;
    friend class BST;
    friend class DLL;
    bool isRevealed;
    string data;
    Structure *parent;
    int loc;
public:
    Cell();
    Cell(string data, Structure *s, bool revealed, int lc);
    ~Cell();
    int reveal(); // show pointer in terminal and update reveal
    string getData();
    bool getRevealed();
    string getId();
    vector<Cell*> getSiblings();
    int getLoc();
};

#endif //CISC220FINAL_UNIT_H
