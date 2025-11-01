#include <iostream>
#include "Unit.h"
using std::string;

class Structure{
    int length;
    string type;
    Unit *members[];
public:
    Structure();
    ~Structure();
};
