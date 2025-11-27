//
// Created by Frank Murphy on 9/23/25.
//

#ifndef CISC220FINAL_UI_HPP
#define CISC220FINAL_UI_HPP
#include <functional>
#include <string>

#include "../external/SDL3_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_pixels.h"

struct AppState;

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
   bool containsPoint(int px, int py) const;
   void setVisible(bool isVisible);
   bool isVisible() const;

   virtual void draw(SDL_Renderer *renderer, TTF_Font *font) = 0;
   virtual void updateCache(SDL_Renderer *renderer, TTF_Font *font) = 0;
   virtual void onMouseMotion(int x, int y) {}
   virtual void onMouseDown(int x, int y, AppState *appstate) {}
   virtual void onMouseUp(int x, int y, AppState *appstate) {}
};

class TextBox : public UIElement {
protected:
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
   std::string getText() const;
   void setText(std::string text);
};
class Terminal {
protected:
   SDL_Color textColor;
   int numitems;
   std::vector<TextBox*> textBoxes;
public:
   Terminal(SDL_Color textColor, SDL_Color backgroundColor, float xScale, float yScale, float wScale,
    float hScale, int numItems);
   void addLine(std::string text);
   TextBox *getLine(int index);
};
class Button : public TextBox {
   using Callback = std::function<void(AppState*)>;
public:
   static constexpr Uint64 CLICK_EFFECT_DURATION_MS = 120;
   enum class ButtonState { Idle, Hovered, Down, Clicked };
   Button(std::string text, SDL_Color textColor, SDL_Color backgroundColor, SDL_Color hoverColor, float xScale,
   float yScale, float wScale, float hScale, Callback onClick = nullptr, Callback onPressImmediate = nullptr);
   void draw(SDL_Renderer *renderer, TTF_Font *font) override;
   void onMouseMotion(int x, int y) override;
   void onMouseDown(int x, int y, AppState *appstate) override;
   void onMouseUp(int x, int y, AppState *appstate) override;
   void updateEffect(int x, int y);
   void onPressImmediate(AppState *appstate);
   void onPress(AppState *appstate);
   ButtonState getState();
private:
   ButtonState state;
   Uint64 clickEffectStart = 0;
   SDL_Color hoverColor{};
   SDL_Color pressedColor{};
   Callback fnOnClick;
   Callback fnOnPressImmediate;
};
#endif //CISC220FINAL_UI_HPP