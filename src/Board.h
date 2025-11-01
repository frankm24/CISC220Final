#include "Unit.h"
#include "Player.h"
class Board{
    Unit grid[256];
    Player players[4]; //mulitplayer implementation
    //single player implementation below:
    int location; //"selected" - "0x##"
    int revealed;
    int moves;

public:
    Board();
    ~Board();
    void generateBoard();
    void printBoard();
//    unlockUnit();
};