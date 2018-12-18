#ifndef GameStart__H
#define GameStart__H

#include <iostream>
#include <string>
#include <SDL.h>
#include "vector2f.h"

class GameStart {
public:
  static GameStart& getInstance();
  bool isVisible() const { return visible; }
  void setVisibility(bool v) { visible = v; }
  void draw();

private:
  int width;
  int height;
  Vector2f pos;
  bool visible;

  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;

  GameStart();
  GameStart(const GameStart&);
  GameStart& operator=(const GameStart&);
};
#endif
