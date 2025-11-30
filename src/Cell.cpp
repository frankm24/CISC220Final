#include <iostream>
#include "../include/Cell.hpp"
#include "../include/Structure.hpp"

Cell::Cell() {
    data = "";
    parent = nullptr;
    isRevealed = false;
}

Cell::Cell(string value, Structure *s, bool revealed ) {
    data = value;
    parent = s;
    isRevealed = revealed;
}

Cell::~Cell() {
 // not sure if this is necessary yet
}

void Cell::reveal() {

}

string Cell::getData() {
    return data;
}

