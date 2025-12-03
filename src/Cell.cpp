#include <iostream>
#include "../include/Cell.hpp"
#include "../include/Structure.hpp"

Cell::Cell() {
    data = "";
    parent = nullptr;
    isRevealed = false;
}

Cell::Cell(string value, Structure *s, bool revealed, int lc) {
    data = value;
    parent = s;
    isRevealed = revealed;
    loc = lc;
}

Cell::~Cell() {
 // not sure if this is necessary yet
}

int Cell::reveal() {
    if (isRevealed) return 0;
    isRevealed = true;
    return 1;
}

string Cell::getData() {
    return data;
}


bool Cell::getRevealed() {
    return isRevealed;
}

string Cell::getId() {
    return parent->getId();
}

int Cell::getLoc() {
    return loc;
}


vector<Cell*> Cell::getSiblings() {
    return parent->members;
}

string Cell::getType() {
    if (data.empty()) return "X";
    if (data.length()==1 && isalpha(data.at(0))) return "C";
    if (data.length()==4 && data.substr(0,2)=="0x") return "P";
    if (data == "null") return "N";
    if (isalpha(data.at(0))) return "S";
    if (data.find('.') != std::string::npos) return "F";
    if (isdigit(data.at(0)) || data.at(0)=='-') return "I";
    return "X";
}