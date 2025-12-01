//
// Created by Maya :) on 11/3/2025.
//

#ifndef CISC220FINAL_PLAYER_H
#define CISC220FINAL_PLAYER_H


class Player{
    friend class Player;
    int location; //"selected" - "0x##"
    int moves;
    int color;
public:
    Player();
    int movePlayer(int loc);
    int getMoves();
    int getLocation();
};

#endif //CISC220FINAL_PLAYER_H