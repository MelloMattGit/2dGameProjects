#ifndef HUD__H
#define HUD__H

#include <iostream>
#include <string>
#include <SDL.h>
#include "vector2f.h"

class Hud {
public:
  static Hud& getInstance();

  const std::string& getText() const { return text; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  const Vector2f& getPosition() const { return pos; }
  SDL_Color getBackgroundColor() const { return backgroundColor; }
  SDL_Color getOutlineColor() const { return outlineColor; }
  SDL_Color getTextColor() const { return textColor; }
  bool isVisible() const { return visible; }

  void setText(const std::string& t) { text = t; }
  void setWidth(int w) { width = w; }
  void setHeight(int h) { height = h; }
  void setPosition(const Vector2f& p) { pos = p; }
  void setBackgroundColor(const SDL_Color& c) { backgroundColor = c; }
  void setOutlineColor(const SDL_Color& c) { outlineColor = c; }
  void setTextColor(const SDL_Color& c) { textColor = c; }
  void setVisibility(bool v) { visible = v; }

  void draw();

private:
  std::string text;
  int width;
  int height;
  Vector2f pos;
  bool visible;
  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;

  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
};
#endif
