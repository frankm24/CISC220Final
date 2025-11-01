#include "unit.h"
#include "Structure.h"
    Unit::Unit(string value, Structure *s, bool revealed ) {
        data = value;
        parent = s;
        isRevealed = revealed;
    }
