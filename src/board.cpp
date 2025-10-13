#include "unit.h" 
#include "player.h" 
#include "board.h"
#include <cstdlib> // For rand and srand
#include <ctime> // For time()

board::board(int s):size(s){
    srand(static_cast<unsigned>(time(0)));
        grid = new unit*[s*s];
        for(int i=0;i<s*s;i++){
            int randomType = rand() % 4; // what data type (int, char, bool, or float) 1-4 respectively
            int randomComplex = rand() % 4; // complex or not (0-2 not complex and 3 complex, this makes it weighted so there are fewer complex units)
                if( randomComplex){
                    int randomComplexType = rand() % 5; // random complex types (array, DLL, BT, Hash, matrix or someother types)
                        switch randomComplexType //descriminate to figure out type 
                             grid[i]=nullptr; // replace with randomly complex unit of randomType. for complex units, iterate through board and set the first null pointer to the adress or something similar
                } else {
                    grid[i]=nullptr; // replace with unit of randomType
                }
            
        }
}
board::~board(){
    for(int i = 0; i < size; i++){
        delete [] grid[i];
    }
    delete [] grid;
}
