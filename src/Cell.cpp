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

// vector<string> Cell::revealParent() {
//     int newRevealed = 0;
//     vector<string> out;
//     for (int i =0; i < parent->size; i++) {
//         if (parent->members[i]->reveal()) newRevealed++;
//         out.push_back(convertToHex(loc)+" data: "+data+"\n");
//     }
//     out.insert(out.begin(), to_string(newRevealed));
//     return out;
// }

vector<Cell*> Cell::getSiblings() {
    return parent->members;
}
