#include "player.h" 
union Type{
    int x;
    char y;
    bool z;
}
class unit {
    bool revealed = false;
    player *owner = nullptr;
    bool isComplex;
    type value;
    unit *ptr1?;
    unit *ptr2?;

public:
    ~unit() {}
    unit(Type v, bool complexity){}
    void setOwner(player *p) { owner = p; }
    player* getOwner() const { return owner; }

    void reveal() { revealed = true; }
    bool isRevealed() const { return revealed; }


    Type PrintValue(){}
    bool getComplexity(){}

    void display() 
}; 