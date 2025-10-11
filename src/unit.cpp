class unit {
protected:
    bool revealed = false;
    player *owner = nullptr;

public:
    virtual ~unit() {} // Virtual destructor for safe polymorphic deletion

    void setOwner(player *p) { owner = p; }
    player* getOwner() const { return owner; }

    void reveal() { revealed = true; }
    bool isRevealed() const { return revealed; }

    virtual void printValue() const = 0; // Pure virtual
    virtual void display() const = 0;    // Pure virtual
};

class integerUnit : public unit{
    int value;
    public:
    integerUnit(int v):value(v){}
    void printValue() const override {std::cout<<value;}
    void display() const override {if(isRevealed())printValue(); else std::cout<<"*";}
};
class CharUnit : public unit{
    //no additional fields
};
