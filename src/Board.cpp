#include "Unit.h"
#include "Player.h" 
#include "Board.h"
#include "Structure.h"

#include <iostream>
#include <cstdlib> // For rand and srand
#include <ctime> // For time()

using std::cout;
using std::endl;

Board::Board() {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated Boards aren't always generated the same
    my_player = Player(); //selected player on local machine
    num_revealed = 0;
    generateBoard(); //populates structures
}

Board::Board(Player player, int revealed) {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated Boards aren't always generated the same
    my_player = player; //selected player on local machine
    num_revealed = revealed;
    generateBoard(); //populates structures
}

Board::Board(Unit board[], Structure** structs, Player player, int revealed) {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated Boards aren't always generated the same
    for (int i = 0; i<256; i++) {
        grid[i] = board[i];
    }
    my_player = player; //selected player on local machine
    num_revealed = revealed;
    structures = structs;
}

Board::~Board(){
    for (int i = 0; structures[i]; i++) {
        delete structures[i]; // not sure if this is the best way to do this
    }
}

void Board::generateBoard() {

}

void Board::printBoard(){
    for (int i = 0; i<256; i++) {
        if (i%16==0) cout << endl;
        cout << grid[i].data << endl;
    }
}

/*Dereferences *dereferences is a very confusing word to use for me in this context in game pointer
 * updates player location to loc on the Board
 * displays all units in the same object
 * returns number of units revealed or -1 if loc is out of bounds
 */
int Board::unlockObject(int loc) {
    if (loc > 255 || loc < 0) {
        return -1;
    }

}
/*Implementation of guess functionality
Allows users to unlock more Units by displaying knowledge of data structure (pseudo code or guess button)
* displays all Units in the same data structure found after Unit at location loc (need to consider implimentation for DLL, Binary Heap, different or same?)
 */
int Board::unlockUnit(int loc){

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


