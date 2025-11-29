#include "SDL3/SDL.h"

#define SDL_MAIN_USE_CALLBACKS
#include <vector>

#include "murphy_util.hpp"
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
enum class UIState { MainMenu, Singleplayer };
struct AppState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    std::vector<UIElement*> main_menu_els;
    std::vector<UIElement*> sp_menu_els;
    std::vector<UIElement*> *active_els = &main_menu_els;
    TerminalInput *terminalInput;
    Terminal *terminal;
    TTF_Font *ui_font = nullptr;
    float dpiScaleX = 1.0f;
    float dpiScaleY = 1.0f;
    int mouseX = 0;
    int mouseY = 0;
    UIState ui_state = UIState::MainMenu;
};

void spOnClick(AppState *state) {
    state->active_els = &state->sp_menu_els;
    for (UIElement *el : *state->active_els) {
        int drawableW, drawableH;
        SDL_GetRenderOutputSize(state->renderer, &drawableW, &drawableH);
        el->computeBounds(drawableW, drawableH);
        el->updateCache(state->renderer, state->ui_font);
    }
    SDL_StartTextInput(state->window);
}

std::string parseCommand(std::string command) {
    return "Successfully did thing";
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
        .06, .1, .48, .8);
    //Adds squares, a white box that is behind the actual squares.
    newstate->sp_menu_els.push_back(squares);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            TextBox *square = new TextBox("u",{0, 0, 0, 0},{0, 0, 255, 0},
                (0.06 + (0.03*j)), (0.1 + (0.05 * i)), .03, .05);
            //Adds a 16x16 grid of squares
            newstate->sp_menu_els.push_back(square);
        }
    }
    // Labels
    for (int i = 0; i < 16; i++) {
        TextBox *square = new TextBox(std::string() + toHexDigit(i),{255, 255, 255, 0},{0, 0, 0, 0},
            (0.01), (0.85 - (0.05 * i)), .05, .05);
        newstate->sp_menu_els.push_back(square);
    }
    for (int i = 0; i < 16; i++) {
        TextBox *square = new TextBox(std::string() + toHexDigit(i),{255, 255, 255, 0},{0, 0, 0, 0},
            (0.06 + (0.03*i)), 0.9, .03, .05);
        newstate->sp_menu_els.push_back(square);
    }
    TextBox *terminalLabel = new TextBox("Terminal:", {255, 255, 255, 0}, {0, 0, 0, 0},
        .6, .1, .35, .05);
    newstate->sp_menu_els.push_back(terminalLabel);

    int numItems = 12;
    Terminal *terminal = new Terminal({255, 255, 255, 0}, {0, 0, 0, 0},
        .6, .13, .35, .4, numItems);
    newstate->terminal = terminal;
    for (int i = 0; i < numItems; i++) {
        terminal->addLine("Identified hashmap");
        newstate->sp_menu_els.push_back(terminal->getLine(i));
    }
    terminal->addLine("Identified array");
    TerminalInput *inputBox = new TerminalInput("player$ ", {255, 255, 255, 0},
        {0, 0, 0, 0}, .6, .6, .35, .05, terminal);
    inputBox->commandParser = parseCommand;
    newstate->sp_menu_els.push_back(inputBox);
    newstate->terminalInput = inputBox;

    TextBox *title = new TextBox("Memsweeper", {255, 255, 255, 255},
        {0, 0, 0, 0}, 0, 0.2, 1, 0.2);
    newstate->main_menu_els.push_back(title);

    Button *spButton = new Button("Singleplayer", {255, 255, 255, 255}, {100, 0, 0, 255}, {200, 200, 100, 255}, 0, 0.5, 1, 0.1, spOnClick);
    newstate->main_menu_els.push_back(spButton);
    for (UIElement *el : newstate->main_menu_els) {
        el->computeBounds(drawableW, drawableH);
        el->updateCache(newstate->renderer, newstate->ui_font);
    }
    for (UIElement *el : newstate->sp_menu_els) {
        el->computeBounds(drawableW, drawableH);
        el->updateCache(newstate->renderer, newstate->ui_font);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // General event handling function
    AppState *state = static_cast<AppState*>(appstate);
    const float x_win = event->motion.x;
    const float y_win = event->motion.y;
    const int x = static_cast<int>(x_win * state->dpiScaleX);
    const int y = static_cast<int>(y_win * state->dpiScaleY);
    state->mouseX = x;
    state->mouseY = y;
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int drawableW, drawableH;
            SDL_GetRenderOutputSize(state->renderer, &drawableW, &drawableH);

            for (UIElement *el : *state->active_els) {
                el->computeBounds(drawableW, drawableH);
                el->updateCache(state->renderer, state->ui_font);
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            for (UIElement *el : *state->active_els) {
                if (!el->isVisible()) continue;
                el->onMouseMotion(x, y);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            for (UIElement *el : *state->active_els) {
                if (!el->isVisible()) continue;
                el->onMouseDown(x, y, state);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            for (UIElement *el : *state->active_els) {
                if (!el->isVisible()) continue;
                el->onMouseUp(x, y, state);
            }
            break;
        }
        case SDL_EVENT_TEXT_INPUT: {
            state->terminalInput->addChars(event->text.text);
            state->terminalInput->updateCache(state->renderer, state->ui_font);
            break;
        }
        case SDL_EVENT_KEY_DOWN:
            if (state->active_els == &state->sp_menu_els) {
                if (event->key.key == SDLK_BACKSPACE) {
                    state->terminalInput->handleBackspace();
                }
                else if (event->key.key == SDLK_RETURN) {
                    state->terminalInput->parseCommand();
                    state->terminal->updateCache(state->renderer, state->ui_font);
                }
                state->terminalInput->updateCache(state->renderer, state->ui_font);
            }
            break;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { // Runs once every main loop
    AppState *state = static_cast<AppState*>(appstate);
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer); // Clears backbuffer, set all pixels to draw color

    for (UIElement* el : *state->active_els) {
        el->draw(state->renderer, state->ui_font);
        if (Button* btn = dynamic_cast<Button*>(el)) {
            // el is actually a Button, so we can do its animation effect
            btn->updateEffect(state->mouseX, state->mouseY);
        }
    }

    SDL_RenderPresent(state->renderer); // Displays the buffer we're drawing to on screen
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState*>(appstate);
    if (state) {
        for (UIElement* elem : *state->active_els) {
            delete elem;
        }
        if (state->ui_font) TTF_CloseFont(state->ui_font);
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        delete state;
    }
    TTF_Quit();
}