//
// Created by Frank Murphy on 9/23/25.
//

#include "ui.hpp"

UIElement::UIElement(float xScale, float yScale, float wScale, float hScale, SDL_Color backgroundColor) : xScale(xScale),
    yScale(yScale), wScale(wScale), hScale(hScale), backgroundColor(backgroundColor), rect() {}

void UIElement::computeBounds(int winW, int winH) {
    this->rect = {xScale * static_cast<float>(winW), yScale * static_cast<float>(winH), wScale * static_cast<float>(winW),
        hScale * static_cast<float>(winH)};
}

void UIElement::setVisible(bool isVisible) {
    this->visible = isVisible;
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

void TextBox::updateCache(SDL_Renderer *renderer, TTF_Font *font) {
    if (this->texture) SDL_DestroyTexture(this->texture);
    TTF_SetFontSize(font, static_cast<float>(0.75*this->rect.h));
    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, this->text.c_str(), this->text.size(), this->textColor,
        static_cast<int>(0.9*this->rect.w));
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(this->texture, SDL_SCALEMODE_NEAREST);
    this->textW = static_cast<float>(surface->w);
    this->textH = static_cast<float>(surface->h);
    SDL_DestroySurface(surface);
}

void TextBox::handleEvent(const SDL_Event &event) {

}


