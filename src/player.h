#include "unit.h" 
class player{
    int x;
    int y;
    unit *territory[];
    public:
    player(int x,int y):x(x),y(y){}
    addTerritory(unit *u){
        territory.push_back(u);
        u->setOwner(this);
    }
}