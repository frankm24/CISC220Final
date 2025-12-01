//
// Created by Frank Murphy on 9/23/25.
//

#ifndef CISC220FINAL_UI_HPP
#define CISC220FINAL_UI_HPP
#include <functional>
#include <string>
#include <boost/circular_buffer.hpp>

#include "../external/SDL3_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_pixels.h"

struct AppState;

enum class TextAlignment { Left, Center };

class UIElement {
protected:
   float x_scale_, y_scale_, w_scale_, h_scale_;
   SDL_Color background_color_;
   bool visible_ = true;
   SDL_FRect rect_{};
   UIElement(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color);
public:
   virtual ~UIElement() = default;
   void computeBounds(int win_w, int win_h);
   bool containsPoint(int px, int py) const;
   void setVisible(bool is_visible);
   bool isVisible() const;
   void setColor(SDL_Color color);

   virtual void draw(SDL_Renderer *renderer, TTF_Font *font) = 0;
   virtual void updateCache(SDL_Renderer *renderer, TTF_Font *font) = 0;
   virtual void onMouseMotion(int x, int y) {}
   virtual void onMouseDown(int x, int y, AppState *state) {}
   virtual void onMouseUp(int x, int y, AppState *state) {}
};

class TextBox : public UIElement {
   friend class TextBoxBuilder;
protected:
   SDL_Texture *texture_ = nullptr;
   std::string text_;
   std::string prev_text_;
   SDL_Color text_color_;
   float text_w_ = 0, text_h_ = 0;
   int fixed_font_size_;
   TextAlignment text_align_x_;
   TextBox(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color, std::string text,
      SDL_Color text_color, int font_size, TextAlignment text_align_x);
public:
   ~TextBox() override;

   void draw(SDL_Renderer *renderer, TTF_Font *font) override;
   void updateCache(SDL_Renderer *renderer, TTF_Font *font) override;
   std::string getText() const;
   void setText(std::string text);
};

class TextBoxBuilder {
   std::string text_;
   SDL_Color text_color_{255, 255, 255, 255};
   SDL_Color background_color_{0, 0, 0, 255};
   float x_scale_ = 0.0f;
   float y_scale_ = 0.0f;
   float w_scale_ = 0.1f;
   float h_scale_ = 0.1f;
   int font_size_ = 0;
   TextAlignment text_align_x_ = TextAlignment::Center;
public:
   TextBoxBuilder();

   TextBoxBuilder& text(std::string t);
   TextBoxBuilder& textColor(SDL_Color c);
   TextBoxBuilder& backgroundColor(SDL_Color c);
   TextBoxBuilder& position(float x, float y);
   TextBoxBuilder& size(float w, float h);
   TextBoxBuilder& fontSize(int px);
   TextBoxBuilder& alignX(TextAlignment align_x);

   TextBox* build();
};

class Terminal {
protected:
   SDL_Color text_color_;
   int num_items_;
   TextBox** text_boxes_;
   boost::circular_buffer<std::string> command_history_;
public:
   Terminal(SDL_Color text_color, SDL_Color background_color, float x_scale, float y_scale, float w_scale,
    float h_scale, int num_items);
   // ~Terminal();
   void addLine(std::string text);
   TextBox *getLine(int index);
   void updateCache(SDL_Renderer *renderer, TTF_Font *font);
};

enum class ButtonState { Idle, Hovered, Down, Clicked };

class Button : public TextBox {
   friend class ButtonBuilder;
   using Callback = std::function<void(AppState*)>;
protected:
   ButtonState state_;
   Uint64 click_effect_start_ = 0;
   SDL_Color hover_color_{};
   SDL_Color pressed_color_{};
   Callback fnOnClick;
   Callback fnOnPressImmediate;
   Button(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color, std::string text,
   SDL_Color text_color, int font_size, TextAlignment text_align_x, SDL_Color hover_color,
   Callback onClick = nullptr, Callback onPressImmediate = nullptr);
public:
   static constexpr Uint64 CLICK_EFFECT_DURATION_MS = 120;
   void draw(SDL_Renderer *renderer, TTF_Font *font) override;
   void onMouseMotion(int x, int y) override;
   void onMouseDown(int x, int y, AppState *state) override;
   void onMouseUp(int x, int y, AppState *state) override;
   void updateEffect(int x, int y);
   void onPressImmediate(AppState *state);
   void onPress(AppState *state);
   ButtonState getState();
};

class ButtonBuilder {
   float x_scale_ = 0.0f;
   float y_scale_ = 0.0f;
   float w_scale_ = 0.1f;
   float h_scale_ = 0.1f;
   SDL_Color background_color_{0, 0, 0, 255};
   std::string text_;
   SDL_Color text_color_{255, 255, 255, 255};
   int font_size_ = 0;
   TextAlignment text_align_x_ = TextAlignment::Center;
   SDL_Color hover_color_{128, 128, 128, 255};
   Button::Callback on_click_;
   Button::Callback on_press_immediate_;
public:
   ButtonBuilder();

   ButtonBuilder& position(float x, float y);
   ButtonBuilder& size(float w, float h);
   ButtonBuilder& backgroundColor(SDL_Color c);
   ButtonBuilder& text(std::string t);
   ButtonBuilder& textColor(SDL_Color c);
   ButtonBuilder& fontSize(int px);
   ButtonBuilder& alignX(TextAlignment align_x);
   ButtonBuilder& hoverColor(SDL_Color c);
   ButtonBuilder& onClick(Button::Callback cb);
   ButtonBuilder& onPressImmediate(Button::Callback cb);

   Button* build();
};

class TerminalInput final : public TextBox {
   using Callback = std::function<std::string(AppState*, std::string)>;
   friend class TerminalInputBuilder;
protected:
   std::string input_text_;
   std::string static_text_;
   Terminal *terminal_;
   Uint64 blink_start_ = 0;
   Uint64 typing_timestamp_ = 0;
   bool blink_state_ = false;
   bool typing_state_ = false;
   TerminalInput(float x_scale, float y_scale, float w_scale, float h_scale, SDL_Color background_color,
    std::string text, SDL_Color text_color, int font_size, TextAlignment text_align_x, Terminal *terminal);
   AppState* app_state_ = nullptr;
public:
   void setAppState(AppState* state) { app_state_ = state; }
   static constexpr Uint64 BLINK_DURATION_MS = 500;
   Callback commandParser; // Should return the command's "output" string to be saved to the commandHistory
   void addChars(const char *text);
   void handleBackspace();
   void parseCommand();
   void draw(SDL_Renderer *renderer, TTF_Font *font) override;
};

class TerminalInputBuilder {
   float x_scale_ = 0.0f;
   float y_scale_ = 0.0f;
   float w_scale_ = 0.1f;
   float h_scale_ = 0.1f;
   int font_size_ = 0;
   SDL_Color background_color_{0, 0, 0, 255};
   std::string text_;
   SDL_Color text_color_{255, 255, 255, 255};
   Terminal *terminal_ = nullptr;
   TerminalInput::Callback command_parser_{};
public:
   TerminalInputBuilder();

   TerminalInputBuilder& position(float x, float y);
   TerminalInputBuilder& size(float w, float h);
   TerminalInputBuilder& backgroundColor(SDL_Color c);
   TerminalInputBuilder& staticText(std::string t);
   TerminalInputBuilder& textColor(SDL_Color c);
   TerminalInputBuilder& fontSize(int px);
   TerminalInputBuilder& terminal(Terminal *t);
   TerminalInputBuilder& commandParser(TerminalInput::Callback cb);

   TerminalInput* build();
};


#endif //CISC220FINAL_UI_HPP