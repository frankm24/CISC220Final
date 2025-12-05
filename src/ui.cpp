//
// Created by Frank Murphy on 9/23/25.
//

#include "ui.hpp"

#include <iomanip>
#include <cmath>

#include "util.hpp"
#include "AppState.hpp"
#include "Board.hpp"
#include "game_logic.hpp"

UIElement::UIElement(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color,
    bool has_outline, SDL_Color outline_color) : x_scale_(x_scale), y_scale_(y_scale), w_scale_(w_scale),
    h_scale_(h_scale), background_color_(background_color), has_outline_(has_outline), outline_color_(outline_color) {}

void UIElement::computeBounds(int win_w, int win_h) {
    rect_ = {x_scale_ * static_cast<float>(win_w), y_scale_ * static_cast<float>(win_h), w_scale_ * static_cast<float>(win_w),
        h_scale_ * static_cast<float>(win_h)};
}

bool UIElement::containsPoint(int px, int py) const {
    return px >= rect_.x && py >= rect_.y &&
           px <  rect_.x + rect_.w &&
           py <  rect_.y + rect_.h;
}

void UIElement::setVisible(bool is_visible) {
    visible_ = is_visible;
}

bool UIElement::isVisible() const {
    return visible_;
}

void UIElement::setColor(SDL_Color color) {
    background_color_ = color;
}

void UIElement::setHighlighted(bool highlight) {
    highlighted_ = highlight;
}


TextBox::TextBox(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color,
    std::string text, SDL_Color text_color, int font_size, TextAlignment text_align_x, bool wrap_text,
    TextAlignment text_align_y, bool has_outline, SDL_Color outline_color) :
    UIElement(x_scale, y_scale, w_scale, h_scale, background_color, has_outline, outline_color),
    text_(std::move(text)), text_color_(text_color), fixed_font_size_(font_size), text_align_x_(text_align_x),
    prev_text_(std::move(text)), wrap_text_(wrap_text), text_align_y_(text_align_y) {}

TextBox::~TextBox() {
    if (texture_) SDL_DestroyTexture(texture_);
}

void TextBox::draw(const UIRenderContext& c) {
    if (text_ != prev_text_) updateCache(c);
    SDL_SetRenderDrawColor(c.renderer, background_color_.r, background_color_.g, background_color_.b,
        background_color_.a);
    SDL_RenderFillRect(c.renderer, &rect_);
    if (has_outline_ && !highlighted_) {
        SDL_SetRenderDrawColor(c.renderer, outline_color_.r, outline_color_.g, outline_color_.b, outline_color_.a);
        SDL_FRect outline_rect = rect_;
        float pad = 12.0f;
        outline_rect.x -= pad;
        outline_rect.y -= pad;
        outline_rect.w += pad * 2.0f;
        outline_rect.h += pad * 2.0f;
        SDL_RenderRect(c.renderer, &outline_rect);
    }
    if (highlighted_) {
        SDL_SetRenderDrawColor(c.renderer, 200, 200, 100, 255);
        SDL_FRect outline_rect = rect_;
        float pad = 12.0f;
        outline_rect.x -= pad;
        outline_rect.y -= pad;
        outline_rect.w += pad * 2.0f;
        outline_rect.h += pad * 2.0f;
        SDL_RenderFillRect(c.renderer, &outline_rect);
    }
    if (texture_) {
        SDL_FRect dst;
        dst.w = text_w_;
        dst.h = text_h_;
        switch (text_align_x_) {
            case TextAlignment::Left:
                dst.x = rect_.x;
                break;
            default:
                dst.x = rect_.x + (rect_.w - dst.w) / 2.0f;
                break;
        }
        switch (text_align_y_) {
            case TextAlignment::Top:
                dst.y = rect_.y;
                break;
            default:
                dst.y = rect_.y + (rect_.h - dst.h) / 2.0f;
                break;
        }
        SDL_RenderTexture(c.renderer, texture_, nullptr, &dst);
    }
    prev_text_ = text_;
}

void TextBox::updateCache(const UIRenderContext& c) {
    if (texture_) SDL_DestroyTexture(texture_);
    if (fixed_font_size_ != 0) TTF_SetFontSize(c.font, fixed_font_size_ * c.dpi_scale / UI_REF_SCALE);
    else TTF_SetFontSize(c.font, 0.75f*rect_.h);

    if (!text_.empty()) {
        SDL_Surface *surface;
        if (wrap_text_) surface = TTF_RenderText_Blended_Wrapped(c.font, text_.c_str(), text_.size(),
            text_color_, static_cast<int>(0.95f*rect_.w));
        else surface = TTF_RenderText_Blended(c.font, text_.c_str(), text_.size(), text_color_);
        texture_ = SDL_CreateTextureFromSurface(c.renderer, surface);
        SDL_SetTextureScaleMode(texture_, SDL_SCALEMODE_NEAREST);
        text_w_ = static_cast<float>(surface->w);
        text_h_ = static_cast<float>(surface->h);
        SDL_DestroySurface(surface);
    }
}

std::string TextBox::getText() const {
    return text_;
}

void TextBox::setText(std::string text) {
    text_ = std::move(text);
}

void TextBox::setTextColor(SDL_Color color) {
    text_color_ = color;
}

TextBoxBuilder::TextBoxBuilder() = default;

TextBoxBuilder& TextBoxBuilder::text(std::string t) {
    text_ = std::move(t);
    return *this;
}

TextBoxBuilder& TextBoxBuilder::textColor(SDL_Color c) {
    text_color_ = c;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::backgroundColor(SDL_Color c) {
    background_color_ = c;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::position(float x, float y) {
    x_scale_ = x;
    y_scale_ = y;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::size(float w, float h) {
    w_scale_ = w;
    h_scale_ = h;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::fontSize(int px) {
    font_size_ = px;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::alignX(TextAlignment align_x) {
    text_align_x_ = align_x;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::alignY(TextAlignment align_y) {
    text_align_y_ = align_y;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::wrapText(bool wrap) {
    wrap_text_ = wrap;
    return *this;
}

TextBoxBuilder& TextBoxBuilder::hasOutline(bool outline) {
    has_outline_ = outline;
    return *this;
}

TextBoxBuilder &TextBoxBuilder::outlineColor(SDL_Color c) {
    outline_color_ = c;
    return *this;
}

TextBox* TextBoxBuilder::build() {
    return new TextBox(
        x_scale_,
        y_scale_,
        w_scale_,
        h_scale_,
        background_color_,
        text_,
        text_color_,
        font_size_,
        text_align_x_,
        wrap_text_,
        text_align_y_,
        has_outline_,
        outline_color_
    );
}

Button::Button(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color, std::string text,
    SDL_Color text_color, int font_size, TextAlignment text_align_x, SDL_Color hover_color, Callback onClick,
    Callback onPressImmediate) : TextBox(x_scale, y_scale, w_scale, h_scale, background_color, std::move(text),
    text_color, font_size, text_align_x, false, TextAlignment::Center, false, {}),
    state_(ButtonState::Idle), hover_color_(hover_color) {
        fnOnClick = std::move(onClick);
        fnOnPressImmediate = std::move(onPressImmediate);
        float hoverL = luminance(hover_color);
        if (hoverL > 128.0f) {
            pressed_color_ = adjustBrightness(hover_color, 0.85f);
        } else {
            pressed_color_ = adjustBrightness(hover_color, 1.15f);
        }
        state_ = ButtonState::Idle;
}

ButtonState Button::getState() {
    return state_;
}

void Button::draw(const UIRenderContext& c) {
    SDL_Color color;
    switch (state_) {
        case ButtonState::Idle:
            color = background_color_;
            break;
        case ButtonState::Hovered:
            color = hover_color_;
            break;
        case ButtonState::Down:
        case ButtonState::Clicked:
        default:
            color = pressed_color_;
            break;
    }
    if (has_outline_ && !highlighted_) {
        SDL_SetRenderDrawColor(c.renderer, outline_color_.r, outline_color_.g, outline_color_.b, outline_color_.a);
        SDL_FRect outline_rect = rect_;
        float pad = 12.0f;
        outline_rect.x -= pad;
        outline_rect.y -= pad;
        outline_rect.w += pad * 2.0f;
        outline_rect.h += pad * 2.0f;
        SDL_RenderRect(c.renderer, &outline_rect);
    }
    if (highlighted_) {
        SDL_SetRenderDrawColor(c.renderer, 200, 200, 100, 255);
        SDL_FRect outline_rect = rect_;
        float pad = 12.0f;
        outline_rect.x -= pad;
        outline_rect.y -= pad;
        outline_rect.w += pad * 2.0f;
        outline_rect.h += pad * 2.0f;
        SDL_RenderFillRect(c.renderer, &outline_rect);
    }
    SDL_SetRenderDrawColor(c.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(c.renderer, &rect_);
    if (texture_) {
        SDL_FRect dst;
        dst.w = text_w_;
        dst.h = text_h_;
        dst.x = rect_.x + (rect_.w - dst.w) / 2.0f;
        dst.y = rect_.y + (rect_.h - dst.h) / 2.0f;
        SDL_RenderTexture(c.renderer, texture_, nullptr, &dst);
    }
}

void Button::onMouseMotion(int x, int y) {
    bool mouseInside = containsPoint(x, y);
    if (mouseInside && state_ == ButtonState::Idle)
        state_ = ButtonState::Hovered;
    else if (!mouseInside && state_ != ButtonState::Idle && state_ != ButtonState::Clicked)
        state_ = ButtonState::Idle;
}

void Button::onPress(AppState *state) {
    if (fnOnClick) fnOnClick(state);
    SDL_Log("full press");
}

void Button::onPressImmediate(AppState *state) {
    if (fnOnPressImmediate) fnOnPressImmediate(state);
    SDL_Log("press immediate");
}

void Button::onMouseDown(int x, int y, AppState *state) {
    if (containsPoint(x, y)) {
        state_ = ButtonState::Down;
        onPressImmediate(state);
        // Allows instant feedback before the actual click,
        // combines Carmack strategy with traditional UI
    }
}

void Button::onMouseUp(int x, int y, AppState *state) {
    if (state_ == ButtonState::Down && containsPoint(x, y)) {
        state_ = ButtonState::Clicked;
        click_effect_start_ = SDL_GetTicks();
        onPress(state);
    }
}

void Button::updateEffect(int x, int y) {
    // If click effect active, return to hover after brief flash

    if (state_ == ButtonState::Clicked &&
        SDL_GetTicks() - click_effect_start_ > CLICK_EFFECT_DURATION_MS) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        if (containsPoint(x, y)) {
            state_ = ButtonState::Hovered;
        } else state_ = ButtonState::Idle;
    }
}

ButtonBuilder::ButtonBuilder() = default;

ButtonBuilder& ButtonBuilder::position(float x, float y) {
    x_scale_ = x;
    y_scale_ = y;
    return *this;
}

ButtonBuilder& ButtonBuilder::size(float w, float h) {
    w_scale_ = w;
    h_scale_ = h;
    return *this;
}

ButtonBuilder& ButtonBuilder::backgroundColor(SDL_Color c) {
    background_color_ = c;
    return *this;
}

ButtonBuilder& ButtonBuilder::text(std::string t) {
    text_ = std::move(t);
    return *this;
}

ButtonBuilder& ButtonBuilder::textColor(SDL_Color c) {
    text_color_ = c;
    return *this;
}

ButtonBuilder& ButtonBuilder::fontSize(int px) {
    font_size_ = px;
    return *this;
}

ButtonBuilder& ButtonBuilder::alignX(TextAlignment align_x) {
    text_align_x_ = align_x;
    return *this;
}

ButtonBuilder& ButtonBuilder::hoverColor(SDL_Color c) {
    hover_color_ = c;
    return *this;
}

ButtonBuilder& ButtonBuilder::onClick(Button::Callback cb) {
    on_click_ = std::move(cb);
    return *this;
}

ButtonBuilder& ButtonBuilder::onPressImmediate(Button::Callback cb) {
    on_press_immediate_ = std::move(cb);
    return *this;
}

Button* ButtonBuilder::build() {
    if (w_scale_ <= 0.0f || h_scale_ <= 0.0f) {
       SDL_Log("ButtonBuilder: invalid size");
       return nullptr;
    }
    return new Button(
       x_scale_,
       y_scale_,
       w_scale_,
       h_scale_,
       background_color_,
       text_,
       text_color_,
       font_size_,
       text_align_x_,
       hover_color_,
       on_click_,
       on_press_immediate_
    );
}

Terminal::Terminal(SDL_Color text_color, SDL_Color background_color, float x_scale, float y_scale, float w_scale,
    float h_scale, int num_items) : text_color_(text_color), output_history_(num_items),
        num_items_(num_items), temp_edit_idx_(0) {
    text_boxes_ = new TextBox*[num_items_];
    const float line_h = h_scale / static_cast<float>(num_items_);
    for (int i = 0; i < num_items_; ++i) {
        float line_y = y_scale + h_scale - static_cast<float>(i) * line_h;
        text_boxes_[i] = TextBoxBuilder{}
        .position(x_scale, line_y)
        .size(w_scale, line_h)
        .backgroundColor(background_color)
        .text("")
        .textColor(text_color)
        .alignX(TextAlignment::Left)
        .build();
    }
    resetTempEdits();
}

void Terminal::addLine(std::string text) {
    output_history_.push_front(text);
    for (int i = 0; i < output_history_.size(); i++) {
        getLine(i)->setText(output_history_.at(i));
    }
}

TextBox *Terminal::getLine(int index) {
    return text_boxes_[index];
}

void Terminal::updateCache(const UIRenderContext& c) {
    for (int i = 0; i < num_items_; i++) {
        getLine(i)->updateCache(c);
    }
}

void Terminal::resetTempEdits() {
    temp_edits_ = input_history_;
    temp_edits_.push_back("");
    temp_edit_idx_ = temp_edits_.size() - 1;
}

void Terminal::appendInputHistory(std::string text) {
    input_history_.push_back(text);
    resetTempEdits();
}

void Terminal::pushToCurrentTempEdit(std::string text) {
    temp_edits_[temp_edit_idx_] += text;
}

void Terminal::popFromCurrentTempEdit() {
    temp_edits_[temp_edit_idx_].pop_back();
}

std::string Terminal::getPrevTempEdit() {
    if (temp_edit_idx_ != 0) {
        temp_edit_idx_--;
        return temp_edits_[temp_edit_idx_];
    }
    return temp_edits_[0];
}

std::string Terminal::getNextTempEdit() {
    if (temp_edit_idx_ != temp_edits_.size() - 1) {
        temp_edit_idx_++;
        return temp_edits_[temp_edit_idx_];
    }
    return temp_edits_[temp_edit_idx_];
}

TerminalInput::TerminalInput(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color,
    std::string text, SDL_Color text_color, int font_size, TextAlignment text_align_x, Terminal *terminal) :
    TextBox(x_scale, y_scale, w_scale, h_scale, background_color, text, text_color, font_size, text_align_x, false,
    TextAlignment::Center, false, {}),
    static_text_(text), terminal_(terminal) {}

void TerminalInput::addChars(const char *text) {
    typing_timestamp_ = SDL_GetTicks();
    input_text_ += std::string(text);
    terminal_->pushToCurrentTempEdit(text);
    text_ = static_text_ + input_text_;
}

void TerminalInput::handleBackspace() {
    typing_timestamp_ = SDL_GetTicks();
    if (!input_text_.empty()) {
        input_text_.pop_back();
        terminal_->popFromCurrentTempEdit();
    }
    text_ = static_text_ + input_text_;
}

void TerminalInput::parseCommand() {
    if (input_text_.empty()) return;
    std::string output = commandParser(app_state_, input_text_);
    if (!output.empty()) {
        terminal_->appendInputHistory(input_text_);
        input_text_ = std::string();
        text_ = static_text_;
        terminal_->addLine(output);
    }
}

void TerminalInput::showPrevInput() {
    input_text_ = terminal_->getPrevTempEdit();
    text_ = static_text_ + input_text_;
}

void TerminalInput::showNextInput() {
    input_text_ = terminal_->getNextTempEdit();
    text_ = static_text_ + input_text_;
}

void TerminalInput::draw(const UIRenderContext& c) {
    if (text_ != prev_text_) updateCache(c);
    if (SDL_GetTicks() - typing_timestamp_ > BLINK_DURATION_MS) {
        typing_state_ = false;
    } else {
        typing_state_ = true;
        blink_start_ = SDL_GetTicks();
        blink_state_ = true;
    }
    if (SDL_GetTicks() - blink_start_ > BLINK_DURATION_MS) {
        blink_start_ = SDL_GetTicks();
        blink_state_ = !blink_state_;
    }
    SDL_SetRenderDrawColor(c.renderer, background_color_.r, background_color_.g, background_color_.b,
        background_color_.a);
    SDL_RenderFillRect(c.renderer, &rect_);
    if (has_outline_ && !highlighted_) {
        SDL_SetRenderDrawColor(c.renderer, outline_color_.r, outline_color_.g, outline_color_.b, outline_color_.a);
        SDL_FRect outline_rect = rect_;
        float pad = 12.0f;
        outline_rect.x -= pad;
        outline_rect.y -= pad;
        outline_rect.w += pad * 2.0f;
        outline_rect.h += pad * 2.0f;
        SDL_RenderRect(c.renderer, &outline_rect);
    }
    if (highlighted_) {
        SDL_SetRenderDrawColor(c.renderer, 200, 200, 100, 255);
        SDL_FRect outline_rect = rect_;
        float pad = 12.0f;
        outline_rect.x -= pad;
        outline_rect.y -= pad;
        outline_rect.w += pad * 2.0f;
        outline_rect.h += pad * 2.0f;
        SDL_RenderFillRect(c.renderer, &outline_rect);
    }
    if (texture_) {
        SDL_FRect dst;
        dst.w = text_w_;
        dst.h = text_h_;
        dst.x = rect_.x;
        dst.y = rect_.y + (rect_.h - dst.h) / 2.0f;
        SDL_RenderTexture(c.renderer, texture_, nullptr, &dst);
        if ((blink_state_ || typing_state_) && show_cursor_) {
                SDL_FRect cursor_rect;
                cursor_rect.x = dst.x + dst.w;
                cursor_rect.y = dst.y - 1;
                cursor_rect.h = dst.h + 2;
                cursor_rect.w = 8;
                SDL_SetRenderDrawColor(c.renderer, text_color_.r, text_color_.g, text_color_.b, text_color_.a);
                SDL_RenderFillRect(c.renderer, &cursor_rect);
        }
    }
    prev_text_ = text_;
}

void TerminalInput::showCursor(bool show) {
    show_cursor_ = show;
}

TerminalInputBuilder::TerminalInputBuilder() = default;

TerminalInputBuilder& TerminalInputBuilder::position(float x, float y) {
    x_scale_ = x;
    y_scale_ = y;
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::size(float w, float h) {
    w_scale_ = w;
    h_scale_ = h;
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::backgroundColor(SDL_Color c) {
    background_color_ = c;
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::staticText(std::string t) {
    text_ = std::move(t);
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::textColor(SDL_Color c) {
    text_color_ = c;
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::fontSize(int px) {
    font_size_ = px;
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::terminal(Terminal *t) {
    terminal_ = t;
    return *this;
}

TerminalInputBuilder& TerminalInputBuilder::commandParser(TerminalInput::Callback cb) {
    command_parser_ = std::move(cb);
    return *this;
}

TerminalInput* TerminalInputBuilder::build() {
    if (!terminal_) {
        SDL_Log("TerminalInputBuilder: terminal is null");
        return nullptr;
    }
    if (w_scale_ <= 0.0f || h_scale_ <= 0.0f) {
        SDL_Log("TerminalInputBuilder: invalid size");
        return nullptr;
    }

    // Adjust the constructor call if your TerminalInput signature is different.
    TerminalInput *input = new TerminalInput(
        x_scale_,
        y_scale_,
        w_scale_,
        h_scale_,
        background_color_,
        text_,
        text_color_,
        font_size_,
        TextAlignment::Left,
        terminal_
    );

    input->commandParser = command_parser_;
    return input;
}

void spOnClick(AppState *state) {
    state->current_scene = SceneID::Singleplayer;
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    state->scenes[SceneID::Singleplayer]->init(ctx, state);
    SDL_StartTextInput(state->window);
}

void tutOnClick(AppState *state) {
    state->current_scene = SceneID::Tutorial;
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    state->scenes[SceneID::Tutorial]->init(ctx, state);
    SDL_StartTextInput(state->window);
}

void exitOnClick(AppState *state) {
    exit(0);
}

MainMenuScene::MainMenuScene(AppState *state) {
    TextBox *title = TextBoxBuilder()
    .position(0, 0.2)
    .size(1, 0.2)
    .backgroundColor({0, 0, 0, 255})
    .text("Memsweeper")
    .textColor({255, 255, 255, 255})
    .build();
    elements_.push_back(title);

    Button *sp_button = ButtonBuilder()
        .position(0, 0.5)
        .size(1, 0.1)
        .backgroundColor({100, 0, 0, 255})
        .text("Singleplayer")
        .textColor({255, 255, 255, 255})
        .hoverColor({200, 200, 100, 255})
        .onClick(spOnClick)
        .build();
    elements_.push_back(sp_button);

    Button *tut_button = ButtonBuilder()
        .position(0, 0.65)
        .size(1, 0.1)
        .backgroundColor({0, 0, 100, 255})
        .text("Tutorial")
        .textColor({255, 255, 255, 255})
        .hoverColor({200, 200, 100, 255})
        .onClick(tutOnClick)
        .build();
    elements_.push_back(tut_button);
}

MainMenuScene::~MainMenuScene() {
    for (UIElement *el : elements_) {
        delete el;
    }
}

void MainMenuScene::init(UIRenderContext &c, AppState *state) {
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    int drawable_w, drawable_h;
    SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
    for (UIElement *el : elements_) {
        el->computeBounds(drawable_w, drawable_h);
        el->updateCache(ctx);
    }
}

void MainMenuScene::draw(UIRenderContext &c, AppState *state) {
    for (UIElement *el : elements_) {
        if (el->isVisible()) el->draw(c);
        if (Button* btn = dynamic_cast<Button*>(el)) {
            btn->updateEffect(state->mouse_x, state->mouse_y);
        }
    }
}

void MainMenuScene::handleEvent(const SDL_Event *event, AppState *state) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseMotion(state->mouse_x, state->mouse_y);
            }
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int drawable_w, drawable_h;
            SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
            UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
            for (UIElement *el : elements_) {
                el->computeBounds(drawable_w, drawable_h);
                el->updateCache(ctx);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseDown(state->mouse_x, state->mouse_y, state);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseUp(state->mouse_x, state->mouse_y, state);
            }
            break;
        }
        default:
            break;
    }
}

SingleplayerScene::SingleplayerScene(AppState *state) {
    TextBox *board_bkg = TextBoxBuilder()
        .position(.06, .1)
        .size(.48, .8)
        .backgroundColor({255, 255, 255, 255})
        .textColor({255, 255, 255, 255})
        .text("b")
        .build();
    //Adds board_bkg, a white box that is behind the actual squares.
    elements_.push_back(board_bkg); // number 0
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            TextBox *square = TextBoxBuilder()
                .position(0.06 + 0.03*j, 0.1 + 0.05 * i)
                .size(0.03, 0.05)
                .fontSize(35)
                .backgroundColor({0, 0, 255, 255})
                .text("?")
                .textColor({0, 0, 0, 255})
                .build();
            elements_.push_back(square); // numbers 1-256
        }
    } //Adds a 16x16 grid of squares
    for (int i = 0; i < 16; i++) {
        TextBox *square = TextBoxBuilder()
            .position(0.03, 0.85 - 0.05*i)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text(std::string() + toHexDigit(i))
            .textColor({255, 255, 255, 255})
            .fontSize(40)
            .build();
        elements_.push_back(square); // numbers 257-272
    } // Labels vertical
    for (int i = 0; i < 16; i++) {
        TextBox *square = TextBoxBuilder()
            .position(0.06 + 0.03*i, 0.9)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text(std::string() + toHexDigit(i))
            .textColor({255, 255, 255, 255})
            .fontSize(40)
            .build();
        elements_.push_back(square); // numbers 273-288
    } // Labels horizontal
    TextBox *terminal_label = TextBoxBuilder()
        .position(.6, .1)
        .size(.35, .05)
        .backgroundColor({0, 0, 0, 255})
        .text("* Terminal *")
        .textColor({255, 255, 255, 255})
        .build();
    elements_.push_back(terminal_label); // number 289

    int numItems = 12;
    Terminal *terminal = new Terminal({255, 255, 255, 0}, {0, 0, 0, 0},
        .6, .13, .35, .4, numItems);
    terminal_ = terminal;
    for (int i = 0; i < numItems; i++) {
        terminal->addLine("-------------------------");
        elements_.push_back(terminal->getLine(i)); // numbers 290-301
    }

    TerminalInput *input_box = TerminalInputBuilder()
        .position(0.6, 0.6)
        .size(.35, .05)
        .backgroundColor({0, 0, 0, 255})
        .staticText("player$ ")
        .textColor({255, 255, 255, 255})
        .terminal(terminal)
        .build();
    input_box->commandParser = [this](AppState* state, std::string cmd) {
        return this->onCommandEntered(state, std::move(cmd));
    };
    input_box->setAppState(state);
    elements_.push_back(input_box); // number 302
    terminal_input_ = input_box;

    TextBox *score = TextBoxBuilder()
        .position(0.19, 0.02)
        .size(0.03, 0.05)
        .backgroundColor({0, 0, 0, 255})
        .text("squares explored: " + std::to_string(state->board->getNumRevealed()) + "/256")
        .textColor({255, 255, 255, 255})
        .fontSize(35)
        .build();
    elements_.push_back(score); // number 303
    score_counter_ = score;

    TextBox *moves = TextBoxBuilder()
        .position(0.6, 0.02)
        .size(0.03, 0.05)
        .backgroundColor({0, 0, 0, 255})
        .text("moves left: " + std::to_string(state->board->getPlayer().getMoves()))
        .textColor({255, 255, 255, 255})
        .fontSize(35)
        .build();
    elements_.push_back(moves); // number 304
    move_counter_ = moves;

    dialog_box_ = TextBoxBuilder()  // Thanks for the code Frank :)
        .position(0.6, 0.7)
        .size(0.35, 0.25)
        .backgroundColor({0, 0, 0, 255})
        .textColor({255, 255, 255, 255})
        .fontSize(35)
        .alignX(TextAlignment::Left)
        .alignY(TextAlignment::Top)
        .wrapText(true)
        .hasOutline(true)
        .outlineColor({255, 255, 255, 255})
        .build();
    elements_.push_back(dialog_box_);

    dialog_box_->setText("Commands:      loc(++/--/0x??):  move                id (graph/matrix     /heap/bst/dll):  id                  q:  quit");

    revealCellLogic(state,0); //why doesn't this increment revealed by 1
    revealCellUI(state, 0);
    std::string out = "0x";
    out.push_back(toHexDigit(0/16));
    out.push_back(toHexDigit(0%16));
    terminal->addLine(out + " data: " + state->board->getGrid()[0].getData());

    TextBox* element = dynamic_cast<TextBox*>(elements_[241]);
    element->setText(":)");
}

SingleplayerScene::~SingleplayerScene() {
    for (UIElement *el : elements_) {
        delete el;
    }
}

void SingleplayerScene::init(UIRenderContext &c, AppState *state) {
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    int drawable_w, drawable_h;
    SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
    for (UIElement *el : elements_) {
        el->computeBounds(drawable_w, drawable_h);
        el->updateCache(ctx);
    }
}

void SingleplayerScene::draw(UIRenderContext &c, AppState *state) {
    for (UIElement *el : elements_) {
        if (el->isVisible()) el->draw(c);
        if (Button* btn = dynamic_cast<Button*>(el)) {
            btn->updateEffect(state->mouse_x, state->mouse_y);
        }
    }
}

void SingleplayerScene::handleEvent(const SDL_Event *event, AppState *state) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseMotion(state->mouse_x, state->mouse_y);
            }
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int drawable_w, drawable_h;
            SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
            UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
            for (UIElement *el : elements_) {
                el->computeBounds(drawable_w, drawable_h);
                el->updateCache(ctx);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseDown(state->mouse_x, state->mouse_y, state);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseUp(state->mouse_x, state->mouse_y, state);
            }
            break;
        }
        case SDL_EVENT_TEXT_INPUT: {
            terminal_input_->addChars(event->text.text);
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            if (event->key.key == SDLK_BACKSPACE) {
                terminal_input_->handleBackspace();
            }
            else if (event->key.key == SDLK_RETURN) {
                terminal_input_->parseCommand();
            } else if (event->key.key == SDLK_UP) {
                terminal_input_->showPrevInput();
            } else if (event->key.key == SDLK_DOWN) {
                terminal_input_->showNextInput();
            }
            break;
        }
        default:
            break;
    }
}

void SingleplayerScene::revealCellUI(AppState *state, int index) {
    elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))]->setColor({0,255,0,0});
    dynamic_cast<TextBox*>(elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))])->setText(state->board->getGrid()[index].getType());
    score_counter_->setText("squares explored: " + std::to_string(state->board->getNumRevealed()) + "/256");
}

void SingleplayerScene::movePlayerUI(AppState *state, int index, int old) {
    move_counter_->setText("moves left: " + std::to_string(state->board->getPlayer().getMoves()));
    UIElement *cell = elements_[(std::ceil((256.0-old)/16)*16-(15-old%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText(state->board->getGrid()[old].getType());
    }
    cell = elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText(":)");
    }
}

void SingleplayerScene::idStructureUI(AppState *state, bool success, int loc, std::vector<Cell*> siblings) {
    if (success) {
        UIElement *cell = elements_[(std::ceil((256.0-loc)/16)*16-(15-loc%16))];
        for (Cell* sibling : siblings) {
            if (!sibling->getRevealed()) revealCellUI(state, sibling->getLoc());
        }
        if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
            element->setText(":)");
        }
    }
    move_counter_->setText("moves left: " + std::to_string(state->board->getPlayer().getMoves()));
}

std::string SingleplayerScene::onCommandEntered(AppState *state, std::string cmd) {
    CommandResult res = parseCommandLogic(state, cmd);
    if (res.didId) {
        idStructureUI(state, res.idSuccess, res.idLoc, res.idSiblings);
    }
    if (res.didReveal) {
        revealCellUI(state, res.revealIndex);
    }
    if (res.didMove) {
        movePlayerUI(state, res.newLoc, res.oldLoc);
    }
    return res.message;
}

TutorialScene::TutorialScene(AppState *state) {
    TextBox *board_bkg = TextBoxBuilder()
        .position(.06, .1)
        .size(.48, .8)
        .backgroundColor({255, 255, 255, 255})
        .textColor({255, 255, 255, 255})
        .text("b")
        .build();
    //Adds board_bkg, a white box that is behind the actual squares.
    elements_.push_back(board_bkg); // number 0
    board_ = board_bkg;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            TextBox *square = TextBoxBuilder()
                .position(0.06 + 0.03*j, 0.1 + 0.05 * i)
                .size(0.03, 0.05)
                .fontSize(35)
                .backgroundColor({0, 0, 255, 255})
                .text("?")
                .textColor({0, 0, 0, 255})
                .build();
            elements_.push_back(square); // numbers 1-256
        }
    } //Adds a 16x16 grid of squares
    for (int i = 0; i < 16; i++) {
        TextBox *square = TextBoxBuilder()
            .position(0.03, 0.85 - 0.05*i)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text(std::string() + toHexDigit(i))
            .textColor({255, 255, 255, 255})
            .fontSize(40)
            .build();
        elements_.push_back(square); // numbers 257-272
        labels_.push_back(square);
    } // Labels vertical
    for (int i = 0; i < 16; i++) {
        TextBox *square = TextBoxBuilder()
            .position(0.06 + 0.03*i, 0.9)
            .size(0.03, 0.05)
            .backgroundColor({0, 0, 0, 255})
            .text(std::string() + toHexDigit(i))
            .textColor({255, 255, 255, 255})
            .fontSize(40)
            .build();
        elements_.push_back(square); // numbers 273-288
        labels_.push_back(square);
    } // Labels horizontal
    TextBox *terminal_label = TextBoxBuilder()
        .position(.6, .1)
        .size(.35, .05)
        .backgroundColor({0, 0, 0, 255})
        .text("* Terminal *")
        .textColor({255, 255, 255, 255})
        .build();
    elements_.push_back(terminal_label); // number 289
    terminal_els.push_back(terminal_label);

    int numItems = 12;
    Terminal *terminal = new Terminal({255, 255, 255, 0}, {0, 0, 0, 0},
        .6, .13, .35, .4, numItems);
    terminal_ = terminal;
    for (int i = 0; i < numItems; i++) {
        terminal->addLine("-------------------------");
        TextBox *el = terminal->getLine(i);
        elements_.push_back(el); // numbers 290-301
        terminal_els.push_back(el);
    }

    TerminalInput *input_box = TerminalInputBuilder()
        .position(0.6, 0.6)
        .size(.35, .05)
        .backgroundColor({0, 0, 0, 255})
        .staticText("player$ ")
        .textColor({255, 255, 255, 255})
        .terminal(terminal)
        .build();
    input_box->commandParser = [this](AppState* state, std::string cmd) {
        return this->onCommandEntered(state, std::move(cmd));
    };
    input_box->setAppState(state);
    elements_.push_back(input_box); // number 302
    terminal_els.push_back(input_box);
    terminal_input_ = input_box;

    TextBox *score = TextBoxBuilder()
        .position(0.1, 0.02)
        .size(0.33, 0.05)
        .backgroundColor({0, 0, 0, 255})
        .text("squares explored: " + std::to_string(state->board->getNumRevealed()) + "/256")
        .textColor({255, 255, 255, 255})
        .fontSize(35)
        .build();
    elements_.push_back(score); // number 303
    score_counter_ = score;

    TextBox *moves = TextBoxBuilder()
        .position(0.5, 0.02)
        .size(0.33, 0.05)
        .backgroundColor({0, 0, 0, 255})
        .text("moves left: " + std::to_string(state->board->getPlayer().getMoves()))
        .textColor({255, 255, 255, 255})
        .fontSize(35)
        .build();
    elements_.push_back(moves); // number 304
    move_counter_ = moves;

    dialog_box_ = TextBoxBuilder()
        .position(0.6, 0.7)
        .size(0.35, 0.25)
        .backgroundColor({0, 0, 0, 255})
        .textColor({255, 255, 255, 255})
        .fontSize(35)
        .alignX(TextAlignment::Left)
        .alignY(TextAlignment::Top)
        .wrapText(true)
        .hasOutline(true)
        .outlineColor({255, 255, 255, 255})
        .build();
    elements_.push_back(dialog_box_);

    dialog_box_->setText("Welcome to Memsweeper! Press Enter to advance the tutorial.");

    terminal_input_->showCursor(false);
}

TutorialScene::~TutorialScene() {
    for (UIElement *el : elements_) {
        delete el;
    }
}

void TutorialScene::init(UIRenderContext &c, AppState *state) {
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    int drawable_w, drawable_h;
    SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
    for (UIElement *el : elements_) {
        el->computeBounds(drawable_w, drawable_h);
        el->updateCache(ctx);
    }
}

void TutorialScene::draw(UIRenderContext &c, AppState *state) {
    for (UIElement *el : elements_) {
        if (el->isVisible()) el->draw(c);
        if (Button* btn = dynamic_cast<Button*>(el)) {
            btn->updateEffect(state->mouse_x, state->mouse_y);
        }
    }
}

void TutorialScene::handleEvent(const SDL_Event *event, AppState *state) {
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN: {
            if (event->key.key == SDLK_RETURN) {
                if (slide_ == 0) {
                    dialog_box_->setText("Your goal is to explore every cell of the computer's memory before "
                                         "you run out of moves! (Press Enter again.)");
                    slide_++;
                }
                else if (slide_ == 1) {
                    dialog_box_->setText("The grid is your computer's memory. It has 256 cells, arranged in a "
                                         "16x16 grid.");
                    board_->setHighlighted(true);
                    slide_++;
                } else if (slide_ == 2) {
                    dialog_box_->setText("Each cell has an address from 0x00 to 0xFF.");
                    board_->setHighlighted(false);
                    for (UIElement *el : labels_) {
                        el->setHighlighted(true);
                    }
                    slide_++;
                } else if (slide_ == 3) {
                    dialog_box_->setText("The smile \":)\" shows where you are in memory. You'll move this around to "
                                         "explore new cells.");
                    for (UIElement *el : labels_) {
                        el->setHighlighted(false);
                    }
                    TextBox* element = dynamic_cast<TextBox*>(elements_[241]);
                    element->setText(":)");
                    element->setColor({0, 255, 0, 255});
                    slide_++;
                } else if (slide_ == 4) {
                    dialog_box_->setText("Here is the number of cells you've explored so far. Exploring new cells "
                                         "increases this score.");
                    score_counter_->setHighlighted(true);
                    slide_++;
                } else if (slide_ == 5) {
                    dialog_box_->setText("Here is the number of moves you have left. Every command costs moves. "
                                         "If you run out of moves, it's game over!");
                    score_counter_->setHighlighted(false);
                    move_counter_->setHighlighted(true);
                    slide_++;
                } else if (slide_ == 6) {
                    dialog_box_->setText("You control your player using the terminal.");
                    move_counter_->setHighlighted(false);
                    for (UIElement *el : terminal_els) {
                        el->setHighlighted(true);
                    }
                    slide_++;
                } else if (slide_ == 7) {
                    dialog_box_->setText("Try the command \"loc++\" to move forward one cell. Type it now and "
                                         "press Enter.");
                    for (UIElement *el : terminal_els) {
                        el->setHighlighted(false);
                    }
                    terminal_input_->setHighlighted(true);
                    terminal_input_->showCursor(true);
                    SDL_StartTextInput(state->window);
                    slide_++;
                } else if (slide_ == 8) {
                    terminal_input_->parseCommand();
                } else if (slide_ == 9) {
                    terminal_input_->parseCommand();
                } else if (slide_ == 10) {
                    dialog_box_->setText("You can also move directly to any address with the command \"loc=0xab\". "
                                         "Let's go to address 0x10. Type \"loc=0x10\" and press Enter.");
                    move_counter_->setHighlighted(false);
                    terminal_input_->setHighlighted(true);
                    terminal_input_->showCursor(true);
                    slide_++;
                } else if (slide_ == 11) {
                    terminal_input_->parseCommand();
                } else if (slide_ == 12) {
                    dialog_box_->setText("Memory cells are filled with different kinds of data: character (C), "
                                             "string (S), int (I), pointer (P), and null (N).");
                    dumbMovePlayerUI(state, 0, 16);
                    state->board->getPlayer().setMoves(50);
                    revealCellUI(state, 0xDC); // S
                    revealCellUI(state, 0x3D); // C
                    revealCellUI(state, 0x11); // I
                    revealCellUI(state, 0x91); // P
                    revealCellUI(state, 0x99); // N
                    move_counter_->setText("moves left: 50");
                    slide_++;
                } else if (slide_ == 13) {
                    dialog_box_->setText("When you move to a new cell, its data is printed in the terminal and your "
                                         "score increases. The first cell is automatically printed when you start the "
                                         "game.");
                    hideCellUI(0xDC);
                    hideCellUI(0x3D);
                    hideCellUI(0x11);
                    hideCellUI(0x91);
                    hideCellUI(0x99);
                    printCellData(state, 0x00);
                    terminal_->getLine(0)->setHighlighted(true);
                    slide_++;
                } else if (slide_ == 14) {
                    dialog_box_->setText("Try using \"loc++\" and \"loc--\" five times to explore nearby cells.");
                    terminal_->getLine(0)->setHighlighted(false);
                    terminal_input_->showCursor(true);
                    slide_++;
                } else if (slide_ == 15) {
                    terminal_input_->parseCommand();
                } else if (slide_ == 16) {
                    dialog_box_->setText("There are 5 types of data structures in the game: graph, matrix, binary "
                                         "heap (heap), binary search tree (bst), and doubly-linked list (dll). All "
                                         "data structures take up at least three cells.");
                    slide_++;
                } else if (slide_ == 17) {
                    dialog_box_->setText("Use the \"id\" command to identify a data structure. If you guess "
                        "correctly, the whole structure is revealed in a single move. If you guess wrong, you lose "
                        "5 moves!");
                    slide_++;
                } else if (slide_ == 18) {
                    dialog_box_->setText("Look at these three cells. Two cells contain pointers and one cell "
                                         "contains a string. This could be a node of a doubly linked list!");
                    movePlayerLogic(state, 1);
                    movePlayerUI(state, 1, 0);
                    movePlayerLogic(state, 2);
                    movePlayerUI(state, 2, 1);
                    slide_++;
                } else if (slide_ == 19) {
                    terminal_input_->showCursor(true);
                    dialog_box_->setText("Type \"id dll\" and press Enter to identify the structure.");
                    slide_++;
                } else if (slide_ == 20) {
                    terminal_input_->parseCommand();
                }
                else if (slide_ == 21) {
                    dialog_box_->setText("Looks like you're ready to play! Press Enter to return to Main Menu.");
                    slide_++;
                } else if (slide_ == 22) {
                    state->current_scene = SceneID::MainMenu;
                    this->~TutorialScene();
                    new (this) TutorialScene(state);
                }
            }
            else if (event->key.key == SDLK_BACKSPACE) {
                if (slide_ == 8 || slide_ == 9 || slide_ == 11 || slide_ == 15 || slide_ == 20)
                    terminal_input_->handleBackspace();
            }
            else if (event->key.key == SDLK_UP) {
                if (slide_ == 8 || slide_ == 9 || slide_ == 11 || slide_ == 15 || slide_ == 20)
                    terminal_input_->showPrevInput();
            } else if (event->key.key == SDLK_DOWN) {
                if (slide_ == 8 || slide_ == 9 || slide_ == 11 || slide_ == 15 || slide_ == 20)
                    terminal_input_->showNextInput();
            }
            break;
        }
        case SDL_EVENT_TEXT_INPUT: {
            if (slide_ == 8 || slide_ == 9 || slide_ == 11 || slide_ == 15 || slide_ == 20)
                terminal_input_->addChars(event->text.text);
            break;
        }
        default:
            break;
    }
}

void TutorialScene::printCellData(AppState *state, int index) {
    std::string y = "0x";
    y.push_back(toHexDigit(index/16));
    y.push_back(toHexDigit(index%16));
    y += " data: " + state->board->getGrid()[index].getData();
    terminal_->addLine(y);
}

void TutorialScene::hideCellUI(int index) {
    TextBox *el = dynamic_cast<TextBox*>(elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))]);
    el->setColor({0, 0, 255, 255});
    el->setText("?");
}

void TutorialScene::dumbMovePlayerUI(AppState *state, int index, int old) {
    int moves = state->board->getPlayer().getMoves() - 1;
    state->board->getPlayer().setMoves(moves);
    move_counter_->setText("moves left: " + std::to_string(moves));
    UIElement *cell = elements_[(std::ceil((256.0-old)/16)*16-(15-old%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText("?");
        element->setColor({0, 0, 255, 255});
    }
    cell = elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText(":)");
        element->setColor({0, 255, 0, 255});
    }
}

void TutorialScene::movePlayerUI(AppState *state, int index, int old) {
    move_counter_->setText("moves left: " + std::to_string(state->board->getPlayer().getMoves()));
    UIElement *cell = elements_[(std::ceil((256.0-old)/16)*16-(15-old%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText(state->board->getGrid()[old].getType());
    }
    cell = elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))];
    if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
        element->setText(":)");
        element->setColor({0, 255, 0, 255});
    }
}

void TutorialScene::revealCellUI(AppState *state, int index) {
    elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))]->setColor({0,255,0,0});
    dynamic_cast<TextBox*>(elements_[(std::ceil((256.0-index)/16)*16-(15-index%16))])->setText(state->board->getGrid()[index].getType());
    score_counter_->setText("squares explored: " + std::to_string(state->board->getNumRevealed()) + "/256");
}

void TutorialScene::idStructureUI(AppState *state, bool success, int loc, const vector<Cell *> &siblings) {
    if (success) {
        UIElement *cell = elements_[(std::ceil((256.0-loc)/16)*16-(15-loc%16))];
        for (Cell* sibling : siblings) {
            if (!sibling->getRevealed()) revealCellUI(state, sibling->getLoc());
        }
        if (TextBox* element = dynamic_cast<TextBox*>(cell)) {
            element->setText(":)");
        }
    }
    move_counter_->setText("moves left: " + std::to_string(state->board->getPlayer().getMoves()));
}

std::string TutorialScene::onCommandEntered(AppState *state, std::string cmd) {
    if (slide_ == 8) {
        if (cmd == "loc++") {
            dumbMovePlayerUI(state, 1, 0);
            slide_++;
            dialog_box_->setText("Great! To move backward one cell, type \"loc--\" and press Enter.");
            return "Moved forward!";
        }
        return "";
    }
    if (slide_ == 9) {
        if (cmd == "loc--") {
            dumbMovePlayerUI(state, 0, 1);
            slide_++;
            dialog_box_->setText("Notice how your moves left has decreased by two, one for each move you made.");
            terminal_input_->showCursor(false);
            terminal_input_->setHighlighted(false);
            move_counter_->setHighlighted(true);
            return "Moved backward!";
        }
        return "";
    }
    if (slide_ == 11) {
        if (cmd == "loc=0x10") {
            dumbMovePlayerUI(state, 16, 0);
            slide_++;
            dialog_box_->setText("Nice! \"loc=0xab\" lets you move anywhere in memory in one move.");
            terminal_input_->setHighlighted(false);
            terminal_input_->showCursor(false);
            return "Moved to 0x10!";
        }
        return "";
    }
    if (slide_ == 15) {
        if (cmd == "loc--" || cmd == "loc++") {
            cmd_counter_++;
            CommandResult res = parseCommandLogic(state, cmd);
            if (res.didReveal) {
                revealCellUI(state, res.revealIndex);
            }
            if (res.didMove) {
                movePlayerUI(state, res.newLoc, res.oldLoc);
            }
            if (cmd_counter_ == 5) {
                slide_++;
                for (int i = 0; i < 256; i++) {
                    hideCellUI(i);
                }
                movePlayerUI(state, 0, state->board->getPlayer().getLocation());
                state->board->getPlayer().setMoves(50);
                move_counter_->setText("moves left: 50");
                terminal_input_->showCursor(false);
                dialog_box_->setText("If you recognize a pattern of data in memory, it may be part of a data "
                                     "structure!");
            }
            return res.message;
        }
    }
    if (slide_ == 20) {
        if (cmd == "id dll") {
            CommandResult res = parseCommandLogic(state, cmd);
            if (res.didId) {
                idStructureUI(state, res.idSuccess, res.idLoc, res.idSiblings);
                slide_++;
                dialog_box_->setText("Wow! Look how many cells you revealed!");
                terminal_input_->showCursor(false);
                return "Identified dll";
            }
        }
        return "";
    }
    return "???";
    //CommandResult res = parseCommandLogic(state, cmd);
    // if (res.didId) {
    //     idStructureUI(state, res.idSuccess, res.idLoc, res.idSiblings);
    // }
    // if (res.didReveal) {
    //     revealCellUI(state, res.revealIndex);
    // }
    // if (res.didMove) {
    //     movePlayerUI(state, res.newLoc, res.oldLoc);
    // }
    //return res.message;
}

EndMenuScene::EndMenuScene(AppState *state) {
    TextBox *game_over = TextBoxBuilder()
        .position(.42, 0.05)
        .size(.2,.2)
        .backgroundColor({0,0,0,255})
        .text("Game Over")
        .textColor({255, 255, 255, 255})
        .build();
    title_ = game_over;
    elements_.push_back(game_over);

    TextBox *explored = TextBoxBuilder()
        .position(.4, 0.35)
        .size(.2,.06)
        .backgroundColor({0,0,0,255})
        .text("Completion Percentage: " + std::to_string(state->board->getNumRevealed()/256) + "%")
        .textColor({255, 255, 255, 255})
        .build();
    elements_.push_back(explored);
    explored_ = explored;

    TextBox *result = TextBoxBuilder()
        .position(.42, 0.45)
        .size(.2,.1)
        .backgroundColor({0,0,0,255})
        .text("Incredibly Lame :(")
        .textColor({255, 0, 0, 255})
        .build();
    elements_.push_back(result);
    result_ = result;

    Button *quit = ButtonBuilder()
        .position(0.1, 0.7)
        .size(.3, 0.1)
        .backgroundColor({100, 0, 0, 255})
        .text("Quit")
        .textColor({255, 255, 255, 255})
        .hoverColor({200, 200, 100, 255})
        .onClick(exitOnClick)
        .build();
    elements_.push_back(quit);

    Button *again = ButtonBuilder()
        .position(0.6, 0.7)
        .size(.3, 0.1)
        .backgroundColor({100, 0, 0, 255})
        .text("Again")
        .textColor({255, 255, 255, 255})
        .hoverColor({200, 200, 100, 255})
        .onClick(exitOnClick)
        .build();
    elements_.push_back(again);
}

EndMenuScene::~EndMenuScene() {
    for (UIElement *el : elements_) {
        delete el;
    }
}

void EndMenuScene::init(UIRenderContext &c, AppState *state) {
    UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
    int drawable_w, drawable_h;
    SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
    displayCompletion(state);
    displayResult(state);
    for (UIElement *el : elements_) {
        el->computeBounds(drawable_w, drawable_h);
        el->updateCache(ctx);
    }
}

void EndMenuScene::draw(UIRenderContext &c, AppState *state) {
    for (UIElement *el : elements_) {
        if (el->isVisible()) el->draw(c);
        if (Button* btn = dynamic_cast<Button*>(el)) {
            btn->updateEffect(state->mouse_x, state->mouse_y);
        }
    }
}

void EndMenuScene::handleEvent(const SDL_Event *event, AppState *state) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseMotion(state->mouse_x, state->mouse_y);
            }
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
            int drawable_w, drawable_h;
            SDL_GetRenderOutputSize(state->renderer, &drawable_w, &drawable_h);
            UIRenderContext ctx = UIRenderContext(state->renderer, state->ui_font, state->rdpi_scale);
            for (UIElement *el : elements_) {
                el->computeBounds(drawable_w, drawable_h);
                el->updateCache(ctx);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseDown(state->mouse_x, state->mouse_y, state);
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            for (UIElement *el : elements_) {
                if (!el->isVisible()) continue;
                el->onMouseUp(state->mouse_x, state->mouse_y, state);
            }
            break;
        }
        default:
            break;
    }
}

void EndMenuScene::displayCompletion(AppState *state) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << (state->board->getNumRevealed()/256.0)*100 << "%";
    explored_->setText("Completion Percentage: " + oss.str());
}

void EndMenuScene::displayResult(AppState *state) {
    float result = state->board->getNumRevealed()/256.0*100;
    if (result< 5) {
        result_->setText("Incredibly Lame :(");
    } else if (result< 20) {
        result_->setText("How Dull...");
    } else if (result< 50) {
        result_->setText("OK...");
    } else if (result>=69 && result <70) {
        result_->setText("Nice");
    } else if (result< 75) {
        result_->setText("Now you're cooking");
    } else if (result< 90) {
        result_->setText("Woohoo!! :)");
    } else if (result< 100) {
        result_->setText("Hope you aren't a perfectionist");
    } else if (result== 100) {
        result_->setText("Perfect.");
        result_->setTextColor({0, 255, 0, 255});
        title_->setText("You win!");
    } else {
        result_->setText("How???");
    }
}