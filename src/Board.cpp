#include "Unit.h"
#include "player.h" 
#include "Board.h"
#include <cstdlib> // For rand and srand
#include <ctime> // For time()

board::Board() {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated boards aren't always generated the same
    Unit grid[256];
    generateBoard();
        }
}
board::~board(){
    for(int i = 0; i < 256; i++){
        delete [] grid[i];
    }
    delete [] grid;
}

void board::printBoard(){
    for (int i = 0; i<256; i++) {
        std::cout << grid[i].data << std::endl;
    }
}
/*Updates player location to loc if it's within bounds
 * returns 0 or -1, if it updated player location or not
 */
int board::movePlayer(int loc) {
    if (loc < 256) {
        location = loc;
        return 0;
    }
    return -1;
}

/*Dereferences in game pointer
 * updates player location to loc on the board
 * displays all units in the same object
 * returns number of units revealed or -1 if loc is out of bounds
 */
int board::unlockObject(int loc) {
    if (loc > 255 || loc < 0) {
        return -1;
    }

}
/*Implimentation of guess functionality
Allows users to unlock more Units by displaying knowledge of data structure (pseudo code or guess button)
* displays all Units in the same data structure found after Unit at location loc (need to consider implimentation for DLL, Binary Heap, different or same?)
 */
int board::unlockUnit(int loc){

    int count = 0;
    for(int i = 0; grid[loc]->parent.members[i] != &grid[loc]; i++) { //should stop when unit at loc is found in data structure
        for ( int j = i; j < grid[loc]->parent.length; j++ ) {
            //iterates through remaining members
            grid[loc]->parent.members[j].isRevealed = true;
            count ++:
        }
    }
    return count;
}


