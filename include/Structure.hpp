#pragma once
#include <iostream>
#include "Cell.hpp"
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
