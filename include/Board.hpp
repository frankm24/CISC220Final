#ifndef CISC220FINAL_BOARD_H
#define CISC220FINAL_BOARD_H

class Board {
    friend class Player;
    friend class Cell;
    Cell grid[256];
    Player my_player; //selected player on local machine
    //Player players[4]; //mulitplayer implementation
    int num_revealed;
    Structure** structures;
public:
    Board();
    Board(Player player, int revealed);
    Board(Cell board[], Structure** structs, Player player, int revealed);
    ~Board();
    void generateDefault();
    void generateBoard();
    void printBoard();
    int unlockObject(int loc);
    int unlockUnit(int loc);
};

#endif //CISC220FINAL_BOARD_H