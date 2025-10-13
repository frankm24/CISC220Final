#include "unit.h" 
typedef struct{
    int r;
    int g;
    int b;
} rgb

class player{
    int x;
    int y;
    rgb color;
    unit *territory[];
    int turns;
    public:
    player(int x,int y):x(x),y(y){}
    void addTerritory(unit *u){
        territory.push_back(u);
        u->setOwner(this);
    }
}