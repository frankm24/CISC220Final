//
// Created by Maya on 11/3/2025.
//

#include "../include/Player.hpp"

#include <iostream>
#include <ostream>


Player::Player() {
    std::cout<<"Created a new player"<<std::endl;
    location = 0x00; //"selected" - "0x##"
    moves = 50; // default?
    color = 0x0000FF; // blue
}

/*Updates player location to loc if it's within bounds
 * returns 0 or -1, if it updated player location or not
 */
int Player::movePlayer(int loc) {
    if (loc < 256) {
        location = loc;
        moves--;
        return 0;
    }
    return -1;
}


int Player::getLocation() {
    return location;
}

int Player::getMoves() {
    return moves;
}

int Player::setMoves(int mvs) {
    moves = mvs;
    return moves;
}