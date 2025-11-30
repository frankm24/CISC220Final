#ifndef CISC220FINAL_BOARD_H
#define CISC220FINAL_BOARD_H
#include "Cell.hpp"
#include "../include/Player.hpp"
#include <vector>
using std::vector;

class Board{
    friend class Player;
    friend class Cell;
    Cell grid[256];
    Player my_player; //selected player on local machine
    //Player players[4]; //mulitplayer implementation
    int num_revealed;
    vector<Structure*> structures;
public:
    Board();
    Board(Player player, int revealed);
    Board(Cell board[], vector<Structure*> structs, Player player, int revealed);
    ~Board();
    void generateDefault();
    void generateBoard();
    void printBoard();
    int unlockObject(int loc);
    int unlockUnit(int loc);
    Cell *getGrid();
    int getNumRevealed();
};

#endif //CISC220FINAL_BOARD_H