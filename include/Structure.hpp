#pragma once
#include <iostream>

class Cell;
using std::string;

class Structure{
    friend class Board;
    int length;
    string type;
    Cell** members;
public:
    Structure();
    Structure(string tp);
    ~Structure();
    Structure* generateStructure(string type);
};
