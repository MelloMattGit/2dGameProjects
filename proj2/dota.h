#include <iostream>
#include <SDL2/SDL.h>
#include "vector2f.h"
#include "drawFilledTriangle.h"
#include <vector>

class Dota{
public:
  Dota(SDL_Window*, SDL_Renderer*, SDL_Rect r,
    std::vector<Vector2f> &pts);
  void draw() ;
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Rect square;
  std::vector<Vector2f> points;
//  int width;
//  int height;
};
