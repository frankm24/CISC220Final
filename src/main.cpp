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
    float dpiScaleX = 1.0f;
    float dpiScaleY = 1.0f;
};

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

    if (!SDL_CreateWindowAndRenderer("CISC220 Final", 800, 450, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &newstate->window, &newstate->renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
        delete newstate;
        return SDL_APP_FAILURE;
    }
    *appstate = newstate;
    int winW, winH;
    SDL_GetWindowSize(newstate->window, &winW, &winH);

    int drawableW, drawableH;
    SDL_GetRenderOutputSize(newstate->renderer, &drawableW, &drawableH);

    newstate->dpiScaleX = static_cast<float>(drawableW) / winW;
    newstate->dpiScaleY = static_cast<float>(drawableH) / winH;

    SDL_Log("Logical window %dx%d, Drawable %dx%d, scale=(%f,%f)",
            winW, winH, drawableW, drawableH,
            newstate->dpiScaleX, newstate->dpiScaleY);

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
    TextBox *squares = new TextBox("b",{255, 255, 255, 255},{255, 255, 255, 255},
        .1, .1, .8, .8);
    //Adds squares, a white box that is behind the actual squares.
    newstate->ui_elements.push_back(squares);
    newstate->ui_elements.back()->setVisible(false);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            TextBox *square = new TextBox("u",{0, 0, 0, 0},{0, 0, 255, 0},
                (0.1 + (0.05*j)), (0.1 + (0.05 * i)), .05, .05);
            //Adds a 16x16 grid of squares
            newstate->ui_elements.push_back(square);
            newstate->ui_elements.back()->setVisible(false);
        }
    }

    TextBox *title = new TextBox("Memsweeper", {0, 0, 0, 255},
        {255, 255, 255, 255}, 0, 0.2, 1, 0.2);
    newstate->ui_elements.push_back(title);
    TextBox *subtitle = new TextBox("Play", {0, 0, 0, 255},
        {255, 255, 255, 255}, 0.25, 0.5, 0.5, 0.1);
    //Adds a play button
    newstate->ui_elements.push_back(subtitle);

    for (UIElement *el : newstate->ui_elements) {
        el->computeBounds(drawableW, drawableH);
        el->updateCache(newstate->renderer, newstate->ui_font);
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // General event handling function
    AppState *state = static_cast<AppState*>(appstate);
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int drawableW, drawableH;
            SDL_GetRenderOutputSize(state->renderer, &drawableW, &drawableH);

            for (UIElement *el : state->ui_elements) {
                el->computeBounds(drawableW, drawableH);
                el->updateCache(state->renderer, state->ui_font);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            /* When the mouse is pressed, the program grabs the location of the mouse, and if it is the same as the
             * dimensions of the play button (which I manually entered) then it turns the title and the play button
             * invisible. Then it sets the background(probably unnecessary) and the grid squares to visible. In the
             * future it would be good to determine if the home menu is still visible before setting the menu to
             * invisible.
             */
            int winW, winH;
            SDL_GetWindowSize(state->window, &winW, &winH);
            float x, y;
            SDL_GetMouseState(&x, &y);
            if (x/winW >= 0.25 && x/winW < 0.75 && y/winH >= 0.5 && y/winH < 0.6) {
                state->ui_elements.back()->setVisible(false);
                state->ui_elements[257]->setVisible(false);
                state->ui_elements[0]->setVisible(true);
                for (int i = 1; i < 257; i++) {
                    state->ui_elements[i]->setVisible(true);
                }
            }
        }
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