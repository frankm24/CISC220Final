//
// Created by Frank Murphy on 12/3/25.
//
#include <iostream>

#include "game_logic.hpp"

#include "Board.hpp"
#include "ui.hpp"
#include "util.hpp"

CommandResult parseCommandLogic(AppState* state, std::string command) {
    CommandResult result;
    command=convertToLower(command);
    command=removeSpaces(command);
    std::cout<< command << std::endl;
    if (command=="quit"|| command=="q") {
        state->quit_next_frame = true;
        result.quit = true;
        result.message = "quitting";
        return result;
    }
    if (command.length() < 3) {
        result.message = "invalid input";
        return result;
    }
    if (command.substr(0,3) == "loc") {
        if (!(command.length() == 5 || command.length()==8)) {
            result.message = "invalid loc";
            return result;
        }
        int oldLoc = state->board->getPlayer().getLocation();
        int newLoc = oldLoc;

        if (command.substr(3, 2) == "++") {
            newLoc = oldLoc + 1;
        } else if (command.substr(3, 2) == "--") {
            newLoc = oldLoc - 1;
        } else if (command.substr(3, 3) == "=0x") {
            newLoc = std::stoi(command.substr(6), nullptr, 16);
        } else {
            result.message = "invalid loc";
            return result;
        }

        // logic: move player (+ reveal inside movePlayerLogic)
        std::string msg = movePlayerLogic(state, newLoc);

        result.didMove = true;
        result.oldLoc = oldLoc;
        result.newLoc = newLoc;

        // movePlayerLogic calls revealCellLogic on success,
        // so if it returned a non-empty string, we know we revealed that index.
        if (!msg.empty()) {
            result.didReveal = true;
            result.revealIndex = newLoc;
        }

        result.message = msg;
        return result;
    }
    if (command.substr(0,2) == "id") {
        std::string guess =command.substr(2);
        if (guess != "heap" && guess!="matrix" && guess!="graph" && guess!="bst" && guess!="dll") {
            result.message = "invalid id";
        }
        std::string msg = idStructure(state, guess); // <- handles comparing
        int loc = state->board->getPlayer().getLocation();
        auto& cell = state->board->getGrid()[loc];
        bool success = (cell.getId() == guess);
        std::vector<Cell*> siblings = cell.getSiblings();
        result.didId = true;
        result.idSuccess = success;
        result.idLoc = loc;
        result.idSiblings = siblings;
        result.message = msg;
        if (state->board->getPlayer().getMoves()<=0) {
            state->current_scene = SceneID::EndMenu;
            UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
            state->scenes[state->current_scene]->init(ctx, state);
        }
        return result;
    }
    result.message = "invalid input";
    return result;
}

std::string revealCellLogic(AppState *state, int index) {
    if (state->board->getGrid()[index].getRevealed()) {
        std::string y = "0x";
        y.push_back(toHexDigit(index/16));
        y.push_back(toHexDigit(index%16));

        return y + " data: " + state->board->getGrid()[index].getData();
    }
    state->board->getGrid()[index].reveal();
    state->board->incrementNumRevealed();

    std::string y = "0x";
    y.push_back(toHexDigit(index/16));
    y.push_back(toHexDigit(index%16));

    if (state->board->getNumRevealed()>=256) {
        state->current_scene = SceneID::EndMenu;
        UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
        state->scenes[state->current_scene]->init(ctx, state);
    }

    return y = y +" data: " + state->board->getGrid()[index].getData();
    // return state->terminal->addLine(y);
}

std::string movePlayerLogic(AppState *state, int index) {
    int old = state->board->getPlayer().getLocation();
    if (old == index) {
        return "";
    }
    if (state->board->getPlayer().movePlayer(index)) {
       return "";
    }
    std::string out = "";
    out = revealCellLogic(state, index);
    if (state->board->getPlayer().getMoves()<=0) {
        state->current_scene = SceneID::EndMenu;
        UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
        state->scenes[state->current_scene]->init(ctx, state);
    }

    return out;
}


std::string idStructure(AppState *state, std::string guess) {
    int loc = state->board->getPlayer().getLocation();

    if (state->board->getGrid()[loc].getId()==guess) {
        std::vector<Cell*> siblings = state->board->getGrid()[loc].getSiblings();
        std::vector<string> out;
        for (Cell* sibling : siblings) {
            if (!sibling->getRevealed()) out.push_back(revealCellLogic(state, sibling->getLoc()));
        }

        // Ideally we would set this to a char representing the cell's data type, but I didn't find an easy way to do it
        return "identified " + guess;
    }
    state->board->getPlayer().setMoves(state->board->getPlayer().getMoves()-5);
    return "incorrect id";
}
