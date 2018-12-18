#include "dota.h"

Dota::Dota(SDL_Window* win, SDL_Renderer* r, SDL_Rect rect, std::vector<Vector2f> &pts):
  window(win), renderer(r), square(rect), points(pts) {}

void Dota::draw() {
  //dotalogo hero(color, WIDTH, HEIGHT);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 120, 25, 33, 255);

  SDL_RenderFillRect(renderer, &square);
  //SDL_Rect r = hero.getSquare();
  SDL_RenderPresent(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  DrawFilledTriangle tr1(window, renderer,  points[0],  points[1],
    points[2]);
  DrawFilledTriangle tr2(window, renderer, points[3], points[4],
       points[5]);
  DrawFilledTriangle tr3(window, renderer, points[6],  points[7],
        points[8]);
  tr1.draw();
  tr2.draw();
  tr3.draw();
}
