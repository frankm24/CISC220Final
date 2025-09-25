//
// Created by Frank Murphy on 9/23/25.
//

#ifndef CISC220FINAL_UI_HPP
#define CISC220FINAL_UI_HPP
#include <string>

#include "../external/SDL3_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_pixels.h"

class UIElement {
protected:
   const float xScale, yScale, wScale, hScale;
   SDL_Color backgroundColor;
   bool visible = true;
   SDL_FRect rect{};
public:
   UIElement(float xScale, float yScale, float wScale, float hSacle, SDL_Color backgroundColor);
   virtual ~UIElement() = default;

   void computeBounds(int winW, int winH);
   void setVisible(bool isVisible);

   virtual void draw(SDL_Renderer *renderer, TTF_Font *font) = 0;
   virtual void updateCache(SDL_Renderer *renderer, TTF_Font *font) = 0;
   virtual void handleEvent(const SDL_Event &event) = 0;
};

class TextBox final : public UIElement {
   SDL_Texture *texture = nullptr;
   std::string text;
   SDL_Color textColor;
   float textW = 0, textH = 0;
public:
   TextBox(std::string text, SDL_Color textColor, SDL_Color backgroundColor, float xScale, float yScale, float wScale,
      float hScale);
   ~TextBox() override;

   void draw(SDL_Renderer *renderer, TTF_Font *font) override;
   void updateCache(SDL_Renderer *renderer, TTF_Font *font) override;
   void handleEvent(const SDL_Event &event) override;
};

#endif //CISC220FINAL_UI_HPP