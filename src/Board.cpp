#include "../include/Cell.hpp"
#include "Player.hpp"
#include "../include/Board.hpp"
#include "../include/Structure.hpp"

#include <iostream>
#include <cstdlib> // For rand and srand
#include <ctime> // For time()

using std::cout;
using std::endl;


Board::Board() {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated Boards aren't always generated the same
    my_player = Player(); //selected player on local machine
    num_revealed = 0;
    for (int i = 0; i<256; i++) {
        grid[i] = Cell();
    }
    cout<< "calling generateDefault()"<<endl;
    generateDefault(); //populates structures with default
    //generateBoard(); // populates structures
}

Board::Board(Player player, int revealed) {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated Boards aren't always generated the same
    my_player = player; //selected player on local machine
    num_revealed = revealed;
    generateBoard(); //populates structures
}

Board::Board(Cell board[], vector<Structure*> structs, Player player, int revealed) {
    //    srand(static_cast<unsigned>(time(0))); // this is for randomizing the seed at each run so that the random generated Boards aren't always generated the same
    for (int i = 0; i<256; i++) {
        grid[i] = board[i];
    }
    my_player = player; //selected player on local machine
    num_revealed = revealed;
    structures = structs;
}

Board::~Board(){
    for (int i = 0; i < structures.size(); i++) {
        delete structures[i]; // not sure if this is the best way to do this
    }
    cout<<"I'm all gone :,)"<<endl;
}


void Board::generateDefault() {
    Heap* heap1 = new Heap(16, {"99", "23", "67", "7", "19", "18", "48", "3", "2", "5", "11", "9", "10", "31", "42", "1"});
    Heap* heap2 = new Heap(15, {"asparagus", "cloud", "green", "is", "goat", "horse", "zebra", "pony", "opera", "zylephone", "salsa","original", "salt", "zoo", "zipper"});
    Graph* graph1 = new Graph(16, {{0, -1, 1,1}, {-1, 0,1,1}, {1,1,0,1}, {1,1,1,0}});
    Graph* graph2 = new Graph(25, {{0,10,-1,2,-1},{-1,0,4,-1,-1},{-1,1,0,-1,5},{-1,-1,3,0,-1},{-1,-1,-1,-1,0}});
    BST* bst1  = new BST(33, {new BSTNode("movie", "0xB2", "0x94"),
        new BSTNode("dolphin", "null", "0xEE"),
        new BSTNode("horse", "null", "0xF1"),
        new BSTNode("mosquito", "0xF4", "null"),
        new BSTNode("lipstick", "null", "null"),
        new BSTNode("watch", "0x6A","0xB5"),
        new BSTNode("stamp", "0x09","0x41"),
        new BSTNode("pasta", "null", "0xB8"),
        new BSTNode("plastic", "null", "null"),
        new BSTNode("t-rex", "null", "null"),
        new BSTNode("window", "null", "null")}, "0x4D");
    DLL* dll1 = new DLL(27, {new DLLNode("the", "null", "0x00"),
        new DLLNode("quick", "0x5C", "0xDB"),
        new DLLNode("brown", "0x00", "0xBF"),
        new DLLNode("fox", "0xDB", "0x50"),
        new DLLNode("jumped","0xBF", "0xAB"),
        new DLLNode("over","0x50", "0xE7"),
        new DLLNode("the", "0xAB","0x91"),
        new DLLNode("lazy", "0xE7", "0x97"),
        new DLLNode("dog","0x91", "null")}, "0x5C","0x97");
    BST* bst2 = new BST(24, {new BSTNode("12", "0x59", "0x61"),
        new BSTNode("6","0x53", "0x4A"),
        new BSTNode("4"),
        new BSTNode("8"),
        new BSTNode("15","null","0xDE"),
        new BSTNode("18", "0xA5", "0xA8"),
        new BSTNode("17"),
        new BSTNode("23")}, "0x6E");
    DLL* dll2 = new DLL(18, {new DLLNode("5", "null", "0x56"),
        new DLLNode("26", "0x22", "0x3A"),
        new DLLNode("33", "0x56", "0xA2"),
        new DLLNode("67", "0x3A", "0x9A"),
        new DLLNode("2","0xA2","0xC8"),
        new DLLNode("201", "0x9A", "null")}, "0x22", "0xC8");
    Matrix* matrx1 = new Matrix(42,6, 6, {
        {"1","3","2","5","20","300"},
        {"43","41","64","53","9","12"},
        {"0", "33", "90", "91", "92", "99"},
        {"-33", "200", "19", "67", "76", "81"},
        {"15", "-6", "3", "18", "69", "55"},
        {"1", "3", "2", "5", "20", "3000"}},
        {"0x44", "0x64","0xE1", "0xC2", "0x8B", "0x25"});
    Matrix* matrx2 = new Matrix(15, 5,2,{
        {"frog", "bug"},
        {"hair", "scrunch"},
        {"paper", "pencil"},
        {"resolve", "merge"},
        {"square", "circle"}},
        {"0x0C", "0x0E", "0x20", "0x5E", "0x6D"});
    Matrix* matrx3 = new Matrix(25, 5,4,{
        {"l", "a", "p", "s"},
        {"c","a","t","s"},
        {"r", "a", "i", "n"},
        {"h", "o", "p", "e"},
        {"f", "r","o","g"}},
        {"0x3D", "0xEA", "0xAE", "0xBB", "0xFC"});
    structures.push_back(heap1);
    structures.push_back(heap2);
    structures.push_back(graph1);
    structures.push_back(graph2);
    structures.push_back(bst1);
    structures.push_back(bst2);
    structures.push_back(dll1);
    structures.push_back(dll2);
    structures.push_back(matrx1);
    structures.push_back(matrx2);
    structures.push_back(matrx3);
    cout<<"board exists but not popuated"<<endl;
    int gridIndex = 0;
    for (Structure* structure : structures) {
        for (int i = 0; i < structure->size; i++) {
            if (gridIndex < 256) {
                grid[gridIndex] = *(structure->members[i]);
                gridIndex++;
            }
        }
    }
    cout<<"Populated it kinda maybe...?"<<endl;
    printBoard();
    cout<<"printed!!"<<endl;
}

void Board::generateBoard() {

}

void Board::printBoard(){
    for (int i = 0; i<256; i++) {
        if (i%16==0) cout << endl<<endl;
        cout << grid[i].data<<"  ";
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
    // for(int i = 0; grid[loc].parent->members[i] != &grid[loc]; i++) { //should stop when unit at loc is found in data structure
    //     for ( int j = i; j < grid[loc].parent->size; j++ ) {
    //         //iterates through remaining members
    //         grid[loc].parent->members[j]->isRevealed = true;
    //         count ++;
    //     }
    // }
    return count;
}


//testing below:
Board testDefault = Board();
//not deleting