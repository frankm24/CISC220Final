//
// Created by Frank Murphy on 9/23/25.
//

#include "ui.hpp"
#include "murphy_util.hpp"

UIElement::UIElement(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color) : x_scale_(x_scale),
    y_scale_(y_scale), w_scale_(w_scale), h_scale_(h_scale), background_color_(background_color) {}

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

TextBox::TextBox(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color,
    std::string text, SDL_Color text_color, int font_size, TextAlignment text_align_x)
    : UIElement(x_scale, y_scale, w_scale, h_scale, background_color), text_(std::move(text)),
    text_color_(text_color), fixed_font_size_(font_size), text_align_x_(text_align_x),
    prev_text_(std::move(text)) {}

TextBox::~TextBox() {
    if (texture_) SDL_DestroyTexture(texture_);
}

void TextBox::draw(const UIRenderContext& c) {
    if (!visible_) return;
    if (text_ != prev_text_) updateCache(c);
    SDL_SetRenderDrawColor(c.renderer, background_color_.r, background_color_.g, background_color_.b,
        background_color_.a);
    SDL_RenderFillRect(c.renderer, &rect_);
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
        dst.y = rect_.y + (rect_.h - dst.h) / 2.0f;
        SDL_RenderTexture(c.renderer, texture_, nullptr, &dst);
    }
    prev_text_ = text_;
}

void TextBox::updateCache(const UIRenderContext& c) {
    if (texture_) SDL_DestroyTexture(texture_);
    if (fixed_font_size_ != 0) TTF_SetFontSize(c.font, fixed_font_size_ * c.dpi_scale / UI_REF_SCALE);
    else TTF_SetFontSize(c.font, 0.75f*rect_.h);

    if (!text_.empty()) {
        SDL_Surface *surface = TTF_RenderText_Blended(c.font, text_.c_str(), text_.size(), text_color_);
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
        text_align_x_
    );
}

Terminal::Terminal(SDL_Color text_color, SDL_Color background_color, float x_scale, float y_scale, float w_scale,
    float h_scale, int num_items) : text_color_(text_color), command_history_(num_items),
        num_items_(num_items) {
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
}

// Terminal::~Terminal() {
//     for (int i = 0; i < num_items_; ++i) {
//         if (text_boxes_[i]) delete text_boxes_[i];
//     }
//     delete[] text_boxes_;
// }

void Terminal::addLine(std::string text) {
    command_history_.push_front(text);
    for (int i = 0; i < command_history_.size(); i++) {
        getLine(i)->setText(command_history_.at(i));
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

Button::Button(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color, std::string text,
    SDL_Color text_color, int font_size, TextAlignment text_align_x, SDL_Color hover_color, Callback onClick,
    Callback onPressImmediate) : TextBox(x_scale, y_scale, w_scale, h_scale, background_color, std::move(text),
    text_color, font_size, text_align_x), state_(ButtonState::Idle), hover_color_(hover_color) {
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
    if (!visible_) return;
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

TerminalInput::TerminalInput(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color,
    std::string text, SDL_Color text_color, int font_size, TextAlignment text_align_x, Terminal *terminal) :
    TextBox(x_scale, y_scale, w_scale, h_scale, background_color, text, text_color, font_size, text_align_x),
    static_text_(text), terminal_(terminal) {}

void TerminalInput::addChars(const char *text) {
    typing_timestamp_ = SDL_GetTicks();
    input_text_ += std::string(text);
    text_ = static_text_ + input_text_;
}

void TerminalInput::handleBackspace() {
    typing_timestamp_ = SDL_GetTicks();
    if (!input_text_.empty()) input_text_.pop_back();
    text_ = static_text_ + input_text_;
}

void TerminalInput::parseCommand() {
    if (input_text_.empty()) return;
    std::string output = commandParser(app_state_, input_text_);
    if (!output.empty()) {
        input_text_ = std::string();
        text_ = static_text_ + input_text_;
        terminal_->addLine(output);
    }
}

void TerminalInput::draw(const UIRenderContext& c) {
    if (!visible_) return;
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
    if (texture_) {
        SDL_FRect dst;
        dst.w = text_w_;
        dst.h = text_h_;
        dst.x = rect_.x;
        dst.y = rect_.y + (rect_.h - dst.h) / 2.0f;
        SDL_RenderTexture(c.renderer, texture_, nullptr, &dst);
        if (blink_state_ || typing_state_) {
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



