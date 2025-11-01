#include <iostream>
#include "Unit.h"
using std::string;

class Structure{
    string length;
    Unit *members[];
public:
    Structure();
    ~Structure();
};
