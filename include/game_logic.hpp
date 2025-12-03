//
// Created by Frank Murphy on 12/3/25.
//

#ifndef CISC220FINAL_GAME_LOGIC_HPP
#define CISC220FINAL_GAME_LOGIC_HPP
#include <string>

#include "AppState.hpp"

class Cell;

struct CommandResult {
    std::string message;

    bool quit = false;

    bool didMove = false;
    int oldLoc = -1;
    int newLoc = -1;

    bool didReveal = false;
    int revealIndex = -1;

    bool didId = false;
    bool idSuccess = false;
    int idLoc = -1;
    std::vector<Cell*> idSiblings;
};

std::string movePlayerLogic(AppState *state, int index);
std::string revealCellLogic(AppState *state, int index);
std::string idStructure(AppState *state, std::string guess);
CommandResult parseCommandLogic(AppState *state, std::string command);

#endif //CISC220FINAL_GAME_LOGIC_HPP