//
// Created by Frank Murphy on 9/23/25.
//

#include "ui.hpp"
#include <cmath>

#include "murphy_util.hpp"

UIElement::UIElement(float xScale, float yScale, float wScale, float hScale, SDL_Color backgroundColor) : xScale(xScale),
                                                                                                          yScale(yScale), wScale(wScale), hScale(hScale), backgroundColor(backgroundColor), rect() {}

void UIElement::computeBounds(int winW, int winH) {
    this->rect = {xScale * static_cast<float>(winW), yScale * static_cast<float>(winH), wScale * static_cast<float>(winW),
        hScale * static_cast<float>(winH)};
}

bool UIElement::containsPoint(int px, int py) const {
    return px >= rect.x && py >= rect.y &&
           px <  rect.x + rect.w &&
           py <  rect.y + rect.h;
}

void UIElement::setVisible(bool isVisible) {
    this->visible = isVisible;
}

bool UIElement::isVisible() const {
    return this->visible;
}

TextBox::TextBox(std::string text, SDL_Color textColor, SDL_Color backgroundColor, float xScale, float yScale,
    float wScale, float hScale) : UIElement(xScale, yScale, wScale, hScale, backgroundColor),
    text(std::move(text)), textColor(textColor) {
}

TextBox::~TextBox() {
    if (this->texture) SDL_DestroyTexture(this->texture);
}

void TextBox::draw(SDL_Renderer *renderer, TTF_Font *font) {
    if (!this->visible) return;
    SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b,
        this->backgroundColor.a);
    SDL_RenderFillRect(renderer, &this->rect);
    if (this->texture) {
        SDL_FRect dst;
        dst.w = this->textW;
        dst.h = this->textH;
        dst.x = this->rect.x + (this->rect.w - dst.w) / 2.0f;
        dst.y = this->rect.y + (this->rect.h - dst.h) / 2.0f;
        SDL_RenderTexture(renderer, this->texture, nullptr, &dst);
    }
}
// ignore for now
float computeLargestFontSize(float rectW, float rectH, unsigned long textLength, float baseSize, int advance0, int lineHeight0) {
    float S0 = std::min(
    rectW * baseSize / float(textLength * advance0),
    rectH * baseSize / float(lineHeight0)
    );
    float adv = advance0 * (S0 / baseSize);
    int charsPerLine = std::max(1, int(rectW / adv));
    int lines = std::ceil(float(textLength) / float(charsPerLine));

    float Sfit = rectH * baseSize / float(lines * lineHeight0);
    return Sfit;
}

void TextBox::updateCache(SDL_Renderer *renderer, TTF_Font *font) {
    if (this->texture) SDL_DestroyTexture(this->texture);
    TTF_SetFontSize(font, 0.75f*this->rect.h);
    // New maybe used FONT Sizing Method
    // float baseSize = TTF_GetFontSize(font);
    // int lineHeight0 = TTF_GetFontHeight(font);
    // int advance0 = 0;
    // if (!TTF_GetGlyphMetrics(font, 'M', nullptr, nullptr, nullptr, nullptr, &advance0) != 0) {
    //     SDL_Log("TTF_GlyphMetrics error: %s", SDL_GetError());
    // }
    // float maxSize = computeLargestFontSize(this->rect.w, this->rect.h, this->text.length(), baseSize, advance0, lineHeight0);
    // TTF_SetFontSize(font, maxSize);

    SDL_Surface *surface = TTF_RenderText_Blended(font, this->text.c_str(), this->text.size(), this->textColor);
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST);
    this->textW = static_cast<float>(surface->w);
    this->textH = static_cast<float>(surface->h);
    SDL_DestroySurface(surface);
}

std::string TextBox::getText() const {
    return this->text;
}

void TextBox::setText(std::string text) {
    this->text = std::move(text);
}

Terminal::Terminal(SDL_Color textColor, SDL_Color backgroundColor, float xScale, float yScale, float wScale,
    float hScale, int numItems) : textColor(textColor),
        numitems(numItems) {
    std::vector<TextBox*> textBoxes = {};
    this->textBoxes = textBoxes;
    for (int i = 0; i < numItems; i++) {
        this->textBoxes.push_back(new TextBox("", textColor, backgroundColor, xScale,
            yScale + hScale - (float)i * (hScale/(float)numItems), wScale, hScale/(float)numItems));
    }
}

void Terminal::addLine(std::string text) {
    std::string old;
    for (int i = 0; i < textBoxes.size(); i++) {
        old = textBoxes[i]->getText();
        textBoxes[i]->setText(std::move(text));
        text = old;
    }
}

TextBox *Terminal::getLine(int index) {
    return textBoxes[index];
}

Button::Button(std::string text, SDL_Color textColor, SDL_Color backgroundColor, SDL_Color hoverColor, float xScale,
    float yScale, float wScale, float hScale, Callback onClick, Callback onPressImmediate) : TextBox(text, textColor, backgroundColor, xScale, yScale, wScale,
        hScale) {
    this->fnOnClick = std::move(onClick);
    this->fnOnPressImmediate = std::move(onPressImmediate);
    this->hoverColor = hoverColor;
    float hoverL = luminance(hoverColor);
    if (hoverL > 128.0f) {
        this->pressedColor = adjustBrightness(hoverColor, 0.85f);
    } else {
        this->pressedColor = adjustBrightness(hoverColor, 1.15f);
    }
    this->state = ButtonState::Idle;
}

Button::ButtonState Button::getState() {
    return this->state;
}

void Button::draw(SDL_Renderer *renderer, TTF_Font *font) {
    if (!this->visible) return;
    SDL_Color color;
    switch (this->state) {
        case ButtonState::Idle:
            color = this->backgroundColor;
            break;
        case ButtonState::Hovered:
            color = this->hoverColor;
            break;
        case ButtonState::Down:
        case ButtonState::Clicked:
            color = this->pressedColor;
            break;
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &this->rect);
    if (this->texture) {
        SDL_FRect dst;
        dst.w = this->textW;
        dst.h = this->textH;
        dst.x = this->rect.x + (this->rect.w - dst.w) / 2.0f;
        dst.y = this->rect.y + (this->rect.h - dst.h) / 2.0f;
        SDL_RenderTexture(renderer, this->texture, nullptr, &dst);
    }
}

void Button::onMouseMotion(int x, int y) {
    bool mouseInside = this->containsPoint(x, y);
    if (mouseInside && this->state == ButtonState::Idle)
        this->state = ButtonState::Hovered;
    else if (!mouseInside && this->state != ButtonState::Idle && this->state != ButtonState::Clicked)
        this->state = ButtonState::Idle;
}

void Button::onPress(AppState *appstate) {
    if (fnOnClick) fnOnClick(appstate);
    SDL_Log("full press");
}

void Button::onPressImmediate(AppState *appstate) {
    if (fnOnPressImmediate) fnOnPressImmediate(appstate);
    SDL_Log("press immediate");
}

void Button::onMouseDown(int x, int y, AppState *appstate) {
    if (this->containsPoint(x, y)) {
        this->state = ButtonState::Down;
        this->onPressImmediate(appstate);
        // Allows instant feedback before the actual click,
        // combines Carmack strategy with traditional UI
    }
}

void Button::onMouseUp(int x, int y, AppState *appstate) {
    if (this->state == ButtonState::Down && this->containsPoint(x, y)) {
        this->state = ButtonState::Clicked;
        this->clickEffectStart = SDL_GetTicks();
        this->onPress(appstate);
    }
}

void Button::updateEffect(int x, int y) {
    // If click effect active, return to hover after brief flash

    if (this->state == ButtonState::Clicked &&
        SDL_GetTicks() - this->clickEffectStart > CLICK_EFFECT_DURATION_MS) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        if (this->containsPoint(x, y)) {
            this->state = ButtonState::Hovered;
        } else this->state = ButtonState::Idle;
    }
}



