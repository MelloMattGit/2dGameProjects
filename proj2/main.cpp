cd#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "frameGenerator.h"
#include "dota.h"
//#include "drawFilledTriangle.h"
//#include "dotalogo.cpp"
#include <vector>


const std::string TITLE = "Matt Hall's Dota Logo";
const std::string NAME = "mthall";

const int WIDTH = 640;
const int HEIGHT = 480;

void drawCircle(SDL_Renderer* renderer,
  SDL_Point center, int radius, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      }
    }
  }
}
/*
void drawSquare(SDL_Renderer* renderer,
  SDL_Point center, int sideLen, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for(int w = 0; w < sideLen; w++){
      for(int h = 0; h < sideLen; h++){
        int dx = sideLen - w; // horizontal offset
        int dy = sideLen - h; // vertical offset
        if ((dx*dx + dy*dy) <= (sideLen * sideLen)) {
          SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
        }
      }
    }
  }
  */


void writeName(SDL_Renderer* renderer) {
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  SDL_Color textColor = {0xff, 0, 0, 0};
  SDL_Surface* surface =
    TTF_RenderText_Solid(font, TITLE.c_str(), textColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {20, HEIGHT-40, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}


int main(void) {
  try {
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
      std::cout << "Failed to initialize SDL2" << std::endl;
      return EXIT_FAILURE;
    }
    SDL_Window* window = SDL_CreateWindow(
      TITLE.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WIDTH,
      HEIGHT,
      SDL_WINDOW_SHOWN
    );
    // Apparently we can't use hardware acceleration with
    // SDL_GetWindowSurface
    SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_SOFTWARE
    );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    //SDL_Point center = {320, 240};
    //SDL_Color color = {120,25,33,255};
  //  drawSquare(renderer, center, 180, color);
  SDL_Rect r;
  r.x = WIDTH/2 - HEIGHT/4;
  r.y = HEIGHT/2 - HEIGHT/4;
  r.w = HEIGHT/2;
  r.h = HEIGHT/2;



  //dotalogo hero(color, WIDTH, HEIGHT);
//  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//  SDL_SetRenderDrawColor(renderer, 120, 25, 33, 255);

//  SDL_RenderFillRect(renderer, &r);
  //SDL_Rect r = hero.getSquare();
//  SDL_RenderPresent(renderer);
/*
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawLine(renderer, r.x + (2*r.w)/3, r.y + (r.h)/8, r.x + (7*r.w)/8,
    r.y + (r.h)/8);
  SDL_RenderDrawLine(renderer, r.x + (7*r.w)/8, r.y + (r.h)/8, r.x + (7*r.w)/8,
    r.y + (r.h)/3);
  SDL_RenderDrawLine(renderer, r.x + (2*r.w)/3, r.y + (r.h)/8, r.x + (7*r.w)/8,
    r.y + (r.h)/3);
*/
  Vector2f v0(r.x + (2*r.w)/3, r.y + (r.h)/8);
  Vector2f v1(r.x + (7*r.w)/8, r.y + (r.h)/8);
  Vector2f v2(r.x + (7*r.w)/8, r.y + (r.h)/3);

  //DrawFilledTriangle tr1(window, renderer, v0, v1, v2);
  //tr1.draw();

  Vector2f v3(r.x + (r.w)/3, r.y + (7*r.h)/8);
  Vector2f v4(r.x + (r.w)/8, r.y + (7*r.h)/8);
  Vector2f v5(r.x + (r.w)/8, r.y + (2*r.h)/3);

  //DrawFilledTriangle tr2(window, renderer, v3, v4, v5);
  //tr2.draw();

  Vector2f v6(r.x + (r.w)/8, r.y + (r.h)/8);
  Vector2f v7(r.x + (2*r.w)/3, r.y + (7*r.h)/8);
  Vector2f v8(r.x + (7*r.w)/8, r.y + (2*r.h)/3);

//  DrawFilledTriangle tr3(window, renderer, v6, v7, v8);
//  tr3.draw();

  std::vector<Vector2f> pts;
  pts.emplace_back(v0);
  pts.emplace_back(v1);
  pts.emplace_back(v2);
  pts.emplace_back(v3);
  pts.emplace_back(v4);
  pts.emplace_back(v5);
  pts.emplace_back(v6);
  pts.emplace_back(v7);
  pts.emplace_back(v8);

  Dota logo(window, renderer, r,  pts);
  logo.draw();

    writeName(renderer);
    SDL_RenderPresent(renderer);
    FrameGenerator frameGen(renderer, window, WIDTH, HEIGHT, NAME);
    frameGen.makeFrame();

    SDL_Event event;
    const Uint8* keystate;
    while ( true ) {
      keystate = SDL_GetKeyboardState(0);
      if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  }
  catch (const std::string& msg) { std::cout << msg << std::endl; }
  catch (...) {
    std::cout << "Oops, someone threw an exception!" << std::endl;
  }
  return EXIT_SUCCESS;
}
