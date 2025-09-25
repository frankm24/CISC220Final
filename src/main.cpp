#include "SDL3/SDL.h"

#define SDL_MAIN_USE_CALLBACKS
#include <vector>

#include "ui.hpp"
#include "SDL3/SDL_main.h"

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

struct AppState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    std::vector<UIElement*> ui_elements;
    TTF_Font *ui_font = nullptr;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) { // Cross-platform main function
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS )) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }
    AppState *newstate = new AppState;

    if (!SDL_CreateWindowAndRenderer("CISC220 Final", 800, 450, 0, &newstate->window, &newstate->renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }
    *appstate = newstate;

    if (!TTF_Init()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }
    std::string fontPath = getAssetPath("fonts/ibm.ttf");
    newstate->ui_font = TTF_OpenFont(fontPath.c_str(), 8);
    if (!newstate->ui_font) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }
    // TTF_SetFontHinting(newstate->ui_font,TTF_HINTING_LIGHT_SUBPIXEL);

    TextBox *title = new TextBox("Memsweeper", {0, 0, 0, 255},
        {255, 255, 255, 255}, 0, 0.2, 1, 0.2);
    newstate->ui_elements.push_back(title);

    int winW, winH;
    SDL_GetWindowSize(newstate->window, &winW, &winH);
    for (UIElement *el : newstate->ui_elements) {
        el->computeBounds(winW, winH);
        el->updateCache(newstate->renderer, newstate->ui_font);
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // General event handling function
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // Runs once every main loop
    AppState *state = static_cast<AppState*>(appstate);
    SDL_SetRenderDrawColor(state->renderer, 100, 200, 100, 255);
    SDL_RenderClear(state->renderer); // Clears backbuffer, set all pixels to draw color

    for (UIElement* elem : state->ui_elements) {
        elem->draw(state->renderer, state->ui_font);
    }

    SDL_RenderPresent(state->renderer); // Displays the buffer we're drawing to on screen
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState*>(appstate);
    if (state) {
        for (UIElement* elem : state->ui_elements) {
            delete elem;
        }
        if (state->ui_font) TTF_CloseFont(state->ui_font);
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        delete state;
    }
    TTF_Quit();
}