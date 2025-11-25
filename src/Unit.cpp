#include <iostream>
#include "Unit.h"
#include "Structure.h"

Unit::Unit() {
    data = "";
    parent = nullptr;
    isRevealed = false;
}

Unit::Unit(string value, Structure *s, bool revealed ) {
    data = value;
    parent = s;
    isRevealed = revealed;
}

Unit::~Unit() {
 // not sure if this is necessary yet
}

void Unit::reveal() {

}

