#include "unit.h"
    Unit::Unit(string value, bool c ):value(v), isComplex(c){}
    Unit::void printValue() {std::cout<<value;}
    Unit::void display() {if(isRevealed())printValue(); else std::cout<<"*";}
    Unit::Type printValue(){
        switch (value){
            case (typeid(value).name() == 'i'):
                return value.x;
            case (typeid(value).name() == 'c'):
                return value.y;
            case (typeid(value).name() == 'b'):
                return value.z;            
        }
    }
