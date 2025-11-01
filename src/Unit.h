#include "player.h"
class Unit {
    bool revealed = false;
    player *owner = nullptr;
    string type;

public:
    ~unit() {}
    void setOwner(player *p) { owner = p; }
    player* getOwner() const { return owner; }

    void reveal() { revealed = true; }
    bool isRevealed() const { return revealed; }


    Type PrintValue(){}
    bool getComplexity(){}

    void display() 
}; 