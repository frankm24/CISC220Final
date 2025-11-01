#include "unit.h"
    unit::unit(string value, bool c ):value(v), isComplex(c){}
    unit::void printValue() {std::cout<<value;}
    unit::void display() {if(isRevealed())printValue(); else std::cout<<"*";}
    unit::Type PrintValue(){
        switch (value){
            case (typeid(value).name() == 'i'):
                return value.x;
            case (typeid(value).name() == 'c'):
                return value.y;
            case (typeid(value).name() == 'b'):
                return value.z;            
        }
    }
