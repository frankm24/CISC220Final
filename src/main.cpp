#include "SDL3/SDL.h"

#define SDL_MAIN_USE_CALLBACKS
#include <cmath>
#include <vector>

#include "Board.hpp"
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
    TerminalInput *terminal_input;
    Terminal *terminal;
    TTF_Font *ui_font = nullptr;
    float dpi_scale_x = 1.0f;
    float dpi_scale_y = 1.0f;
    int mouse_x = 0;
    int mouse_y = 0;
    UIState ui_state = UIState::MainMenu;
    Board *board;
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

void revealCell(AppState *state, int index) {
    state->board->getGrid()[index].reveal();
    state->board->incrementNumRevealed();
    state->sp_menu_els[(std::ceil((256.0-index)/16)*16-(15-index%16))]->setColor({0,255,0,0});
    std::string y = "0x";
    y.push_back(toHexDigit(index/16));
    y.push_back(toHexDigit(index%16));
    y = y +" data: " + state->board->getGrid()[index].getData();
    state->terminal->addLine(y);
}

int movePlayer(AppState *state, int index) {
    int old = state->board->getPlayer().getLocation();
    if (old == index) {
        return 0;
    }
    if (state->board->getPlayer().movePlayer(index)) {
       return 0;
    }
    if (TextBox* element = dynamic_cast<TextBox*>(state->sp_menu_els[304])) {
        element->setText("moves left: " + std::to_string(state->board->getPlayer().getMoves()));
        element->updateCache(state->renderer,state->ui_font);
    }
    revealCell(state, index);
    if (TextBox* element = dynamic_cast<TextBox*>(state->sp_menu_els[303])) {
        element->setText("squares explored: " + std::to_string(state->board->getNumRevealed()) + "/256");
        element->updateCache(state->renderer,state->ui_font);
    }
    UIElement *cell = state->sp_menu_els[(std::ceil((256.0-old)/16)*16-(15-old%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText("A");
        // Ideally we would set this to a char representing the cell's data type, but I didn't find an easy way to do it
        element->updateCache(state->renderer,state->ui_font);
    }
    cell = state->sp_menu_els[(std::ceil((256.0-index)/16)*16-(15-index%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText(":)");
        element->updateCache(state->renderer,state->ui_font);
    }
    return 1;
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

    TextBox *squares = TextBoxBuilder()
        .position(.06, .1)
        .size(.48, .8)
        .backgroundColor({255, 255, 255, 255})
        .textColor({255, 255, 255, 255})
        .text("b")
        .build();
    //Adds squares, a white box that is behind the actual squares.
    newstate->sp_menu_els.push_back(squares); // number 0
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            TextBox *square = TextBoxBuilder()
                .position(0.06 + 0.03*j, 0.1 + 0.05 * i)
                .size(0.03, 0.05)
                .fontSize(20)
                .backgroundColor({0, 0, 255, 255})
                .text("?")
                .textColor({0, 0, 0, 255})
                .build();
            //Adds a 16x16 grid of squares
            newstate->sp_menu_els.push_back(square); // numbers 1-256
        }
    }
    // Labels
    for (int i = 0; i < 16; i++) {
        TextBox *square = TextBoxBuilder()
            .position(0.03, 0.85 - 0.05*i)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text(std::string() + toHexDigit(i))
            .textColor({255, 255, 255, 255})
            .fontSize(40)
            .build();
        newstate->sp_menu_els.push_back(square); // numbers 257-272
    }
    for (int i = 0; i < 16; i++) {
        TextBox *square = TextBoxBuilder()
            .position(0.06 + 0.03*i, 0.9)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text(std::string() + toHexDigit(i))
            .textColor({255, 255, 255, 255})
            .fontSize(40)
            .build();
        newstate->sp_menu_els.push_back(square); // numbers 273-288
    }
    TextBox *terminal_label = TextBoxBuilder()
        .position(.6, .1)
        .size(.35, .05)
        .backgroundColor({0, 0, 0, 255})
        .text("* Terminal *")
        .textColor({255, 255, 255, 255})
        .build();
    newstate->sp_menu_els.push_back(terminal_label); // number 289

    int numItems = 12;
    Terminal *terminal = new Terminal({255, 255, 255, 0}, {0, 0, 0, 0},
        .6, .13, .35, .4, numItems);
    newstate->terminal = terminal;
    for (int i = 0; i < numItems; i++) {
        terminal->addLine("-------------------------");
        newstate->sp_menu_els.push_back(terminal->getLine(i)); // numbers 290-301
    }
    TerminalInput *input_box = TerminalInputBuilder()
        .position(0.6, 0.6)
        .size(.35, .05)
        .backgroundColor({0, 0, 0, 255})
        .staticText("player$ ")
        .textColor({255, 255, 255, 255})
        .terminal(terminal)
        .build();
    input_box->commandParser = parseCommand;
    newstate->sp_menu_els.push_back(input_box); // number 302
    newstate->terminal_input = input_box;

    TextBox *score = TextBoxBuilder()
            .position(0.19, 0.02)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text("squares explored: " + std::to_string(newstate->board->getNumRevealed()) + "/256")
            .textColor({255, 255, 255, 255})
            .fontSize(20)
            .build();
    newstate->sp_menu_els.push_back(score); // number 303

    TextBox *moves = TextBoxBuilder()
            .position(0.6, 0.02)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text("moves left: " + std::to_string(newstate->board->getPlayer().getMoves()))
            .textColor({255, 255, 255, 255})
            .fontSize(20)
            .build();
    newstate->sp_menu_els.push_back(moves); // number 304

    TextBox *title = TextBoxBuilder()
        .position(0, 0.2)
        .size(1, 0.2)
        .backgroundColor({0, 0, 0, 255})
        .text("Memsweeper")
        .textColor({255, 255, 255, 255})
        .build();
    newstate->main_menu_els.push_back(title);

    Button *sp_button = ButtonBuilder()
        .position(0, 0.5)
        .size(1, 0.1)
        .backgroundColor({100, 0, 0, 255})
        .text("Singleplayer")
        .textColor({255, 255, 255, 255})
        .hoverColor({200, 200, 100, 255})
        .onClick(spOnClick)
        .build();
    newstate->main_menu_els.push_back(sp_button);

    revealCell(newstate,0);
    if (TextBox* element = dynamic_cast<TextBox*>(newstate->sp_menu_els[241])) {
        element->setText(":)");
    }

    for (UIElement *el : newstate->main_menu_els) {
        el->computeBounds(drawable_w, drawable_h);
        el->updateCache(newstate->renderer, newstate->ui_font);
    }
    for (UIElement *el : newstate->sp_menu_els) {
        el->computeBounds(drawable_w, drawable_h);
        el->updateCache(newstate->renderer, newstate->ui_font);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { // General event handling function
    AppState *state = static_cast<AppState*>(appstate);
    const float x_win = event->motion.x;
    const float y_win = event->motion.y;
    const int x = static_cast<int>(x_win * state->dpi_scale_x);
    const int y = static_cast<int>(y_win * state->dpi_scale_y);
    state->mouse_x = x;
    state->mouse_y = y;
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int drawable_w, drawable_h;
            SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);

            for (UIElement *el : *state->active_els) {
                el->computeBounds(drawable_w, drawable_h);
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
            state->terminal_input->addChars(event->text.text);
            state->terminal_input->updateCache(state->renderer, state->ui_font);
            break;
        }
        case SDL_EVENT_KEY_DOWN:
            if (state->active_els == &state->sp_menu_els) {
                if (event->key.key == SDLK_BACKSPACE) {
                    state->terminal_input->handleBackspace();
                }
                else if (event->key.key == SDLK_RETURN) {
                    state->terminal_input->parseCommand();
                    state->terminal->updateCache(state->renderer, state->ui_font);
                }
                state->terminal_input->updateCache(state->renderer, state->ui_font);
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
            btn->updateEffect(state->mouse_x, state->mouse_y);
        }
    }

    SDL_RenderPresent(state->renderer); // Displays the buffer we're drawing to on screen
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState*>(appstate);
    if (state) {
        delete state->terminal;
        for (UIElement* elem : state->sp_menu_els) {
            delete elem;
        }
        for (UIElement* elem : state->main_menu_els) {
            delete elem;
        }
        if (state->ui_font) TTF_CloseFont(state->ui_font);
        if (state->renderer) SDL_DestroyRenderer(state->renderer);
        if (state->window) SDL_DestroyWindow(state->window);
        if (state->board) delete state->board;
        delete state;
    }
    TTF_Quit();
}