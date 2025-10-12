#include "unit.h"
    unit::unit(Type v, bool c ):value(v), isComplex(c){} 
    unit::void printValue() {std::cout<<value;}
    unit::void display() {if(isRevealed())printValue(); else std::cout<<"*";}
