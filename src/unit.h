#include "player.h" 
union Type{
    int x;
    char y;
    bool z;
    int pointer;
}
class unit {
    bool revealed = false;
    player *owner = nullptr;
    type value;

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