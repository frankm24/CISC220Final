//
// Created by Frank Murphy on 12/3/25.
//

#ifndef CISC220FINAL_APPSTATE_HPP
#define CISC220FINAL_APPSTATE_HPP

#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
struct TTF_Font;

enum class SceneID;
class UIScene;
class Board;

struct AppState {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* ui_font = nullptr;

    float dpi_scale_x = 1.0f;
    float dpi_scale_y = 1.0f;
    float rdpi_scale  = 1.0f;

    int mouse_x = 0;
    int mouse_y = 0;

    bool quit_next_frame = false;

    std::unordered_map<SceneID, UIScene*> scenes;
    SceneID current_scene;
    Board *board;
};

#endif //CISC220FINAL_APPSTATE_HPP