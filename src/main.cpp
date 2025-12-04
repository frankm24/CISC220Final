#include "SDL3/SDL.h"

#define SDL_MAIN_USE_CALLBACKS

#include <iostream>
#include <string>

#include "Board.hpp"
#include "ui.hpp"
#include "SDL3/SDL_main.h"
#include "AppState.hpp"

std::string getAssetPath(const std::string &relative) {
    const char *base = SDL_GetBasePath();
    if (!base) {
        // fallback: relative to current working directory
        return "assets/" + relative;
    }

    std::string path(base);
    // Do NOT SDL_free(base) in SDL3!

    path += "assets/" + relative;
    return path;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // Cross-platform main function
    // Get output in CLion to work
    SDL_SetLogOutputFunction([](void *userdata, int category, SDL_LogPriority priority, const char *message){
        fprintf(stderr, "[SDL] %s\n", message);
    }, nullptr);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS )) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }
    AppState *newstate = new AppState;

    if (!SDL_CreateWindowAndRenderer("CISC220 Final", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &newstate->window, &newstate->renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowMinimumSize(newstate->window, 1280, 720);
    if (!SDL_SetRenderVSync(newstate->renderer, 1)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);

    } else SDL_Log("VSync enabled");

    // VSync avoids from the app looping thousands of times per second by syncing it to the screen's refresh
    // the game only loops as fast as it needs to do display the UI at your monitor's refresh rate
    // normally this is a different setting from the game's tick rate but it's not too important right now
    *appstate = newstate;
    int win_w, win_h;
    SDL_GetWindowSize(newstate->window, &win_w, &win_h);

    int drawable_w, drawable_h;
    SDL_GetRenderOutputSize(newstate->renderer, &drawable_w, &drawable_h);

    newstate->dpi_scale_x = static_cast<float>(drawable_w) / win_w;
    newstate->dpi_scale_y = static_cast<float>(drawable_h) / win_h;

    newstate->rdpi_scale = SDL_GetWindowDisplayScale(newstate->window);

    SDL_Log("Logical window %dx%d, Drawable %dx%d, scale=(%f,%f)",
            win_w, win_h, drawable_w, drawable_h,
            newstate->dpi_scale_x, newstate->dpi_scale_y);

    if (!TTF_Init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }
    std::string font_path = getAssetPath("fonts/ibm.ttf");
    newstate->ui_font = TTF_OpenFont(font_path.c_str(), 8);
    if (!newstate->ui_font) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }

    newstate->board = new Board();
    newstate->scenes[SceneID::MainMenu] = new MainMenuScene(newstate);
    newstate->scenes[SceneID::Singleplayer] = new SingleplayerScene(newstate);
    newstate->scenes[SceneID::Tutorial] = new TutorialScene(newstate);
    newstate->scenes[SceneID::EndMenu] = new EndMenuScene(newstate);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // General event handling function
    AppState *state = static_cast<AppState*>(appstate);
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_MOUSE_MOTION: {
            const float x_win = event->motion.x;
            const float y_win = event->motion.y;
            const int x = static_cast<int>(x_win * state->dpi_scale_x);
            const int y = static_cast<int>(y_win * state->dpi_scale_y);
            state->mouse_x = x;
            state->mouse_y = y;
            break;
        }
        default:
            break;
    }
    state->scenes[state->current_scene]->handleEvent(event, state);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // Runs once every main loop
    AppState *state = static_cast<AppState*>(appstate);
    if (state->quit_next_frame) return SDL_APP_SUCCESS;
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer); // Clears backbuffer, set all pixels to draw color
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    state->scenes[state->current_scene]->draw(ctx, state);
    SDL_RenderPresent(state->renderer); // Displays the buffer we're drawing to on screen
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState*>(appstate);
    if (state) {
        for (pair<const SceneID, UIScene*> &pair : state->scenes) {
            delete pair.second;
        }
        if (state->ui_font) TTF_CloseFont(state->ui_font);
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        if (state->board) delete state->board;
        delete state;
    }
    TTF_Quit();
}