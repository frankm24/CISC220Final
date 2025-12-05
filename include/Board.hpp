#ifndef CISC220FINAL_BOARD_H
#define CISC220FINAL_BOARD_H
#include "Cell.hpp"
#include "../include/Player.hpp"
#include <vector>
#include <sstream>
using std::vector;
using namespace std;

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
    Player& getPlayer();
    int getNumRevealed();
    int incrementNumRevealed();
    int setNum_Revealed(int num);
    Board *newBoard(Board *board);
};

// Helper function:
inline string convertToHex(int num) {

    stringstream ss; // Create a stringstream object

    ss << hex << num; // Set to hex format and insert the number

    string hexString = "0x"+ss.str(); // Get the hexadecimal string
    return hexString;
}


#endif //CISC220FINAL_BOARD_H