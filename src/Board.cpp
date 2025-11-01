#include "Unit.h"
#include "player.h" 
#include "Board.h"
#include <cstdlib> // For rand and srand
#include <ctime> // For time()

board::Board() {
    //    srand(static_cast<unsigned>(time(0)));
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

board::printBoard(){
    for (int i = 0; i<256; i++) {
        std::cout << this->grid[i].data << std::endl;
    }
}

//board::unlockUnit(); // to implement, unlocks whole objecy node when dereferncing a pointer
