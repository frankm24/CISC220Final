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

void Cell::reveal() {
    isRevealed = true;
}

string Cell::getData() {
    return data;
}


bool Cell::getRevealed() {
    return isRevealed;
}

void Cell::revealParent(Structure* parent) {
    for (int i =0; i < parent->size; i++) {
        parent->members[i]->reveal();
    }
}

void Cell::idStructure(string input) {
    string temp = input;
    std::transform(temp.begin(), temp.end(), temp.begin(),
               [](unsigned char c){ return std::tolower(c); });
    boost::trim(temp);
    if (temp==parent->id) {
        revealParent(parent);
    }
}