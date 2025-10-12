#include "unit.h" 
#include "player.h" 
#include "board.h"

board::board(int s):size(s){
        grid = new unit*[s*s];
        for(int i=0;i<s*s;i++){
            grid[i]=nullptr;
        }
}
board::~board(){
    for(int i = 0; i < size; i++){
        delete [] grid[i];
    }
    delete [] grid;
}
