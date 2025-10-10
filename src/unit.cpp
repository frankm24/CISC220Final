abstract class unit{
    bool revealed;
    player *owner;
    //I want to somehow add a field for value, but I'm not sure how to make union types in C++ so I'll make it inheritance
    public: 
    void ~unit(){}
    void setOwner(player *p){owner=p;}
    player* getOwner(){return owner;}
    void reveal(){revealed=true;}
    bool isRevealed(){return revealed;}
    void printValue(){}
    void display(){}
};
class integerUnit : public unit{
    int value;
    public:
    integerUnit(int v):value(v){}
    void printValue(){std::cout<<value;}
    void display(){if(isRevealed())printValue(); else std::cout<<"*";}
};
class bombUnit : public unit{
    //no additional fields
};
