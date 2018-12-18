#ifndef GameBoss__H
#define GameBoss__H

#include <iostream>
#include <string>
#include <SDL.h>
#include "vector2f.h"

class GameBoss {
public:
  static GameBoss& getInstance();
  bool isVisible() const { return visible; }
  void setVisibility(bool v) { visible = v; }
  void draw();

private:
  int width;
  int height;
  Vector2f pos;
  int visible;

  SDL_Color backgroundColor;
  SDL_Color outlineColor;
  SDL_Color textColor;

  GameBoss();
  GameBoss(const GameBoss&);
  GameBoss& operator=(const GameBoss&);
};
#endif
