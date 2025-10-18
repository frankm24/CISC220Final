#include "unit.h" 
typedef struct{
    int r;
    int g;
    int b;
} rgb;

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
    void setColor(int r, int g, int b) {
        color = {r, g, b};
    }
    rgb getColor() const {
        return color;
    }
    void incrementTurn() {
        turns++;
    }
    int getTurns() const {
        return turns;
    }

}