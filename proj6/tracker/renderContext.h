#ifndef RENDERCONTEXT__H
#define RENDERCONTEXT__H

#include <iostream>
#include <SDL.h>
#include "imageFactory.h"

class RenderContext{
public:
  static RenderContext* getInstance();
  ~RenderContext();
  SDL_Window* getWindow() const { return window; }
  SDL_Renderer* getRenderer() const { return renderer; }
  Image* getImage(const std::string& n) {
    return ImageFactory::getInstance().getImage(n);
  }
  std::vector<Image*> getImages(const std::string& n) {
    return ImageFactory::getInstance().getImages(n);
}

private:
  static RenderContext* instance;
  SDL_Window* window;
  SDL_Renderer* renderer;

  //ImageFactory &factory;

  SDL_Window* initWindow();
  SDL_Renderer* initRenderer();
  RenderContext();
  RenderContext(const RenderContext&) = delete;
  RenderContext& operator=(const RenderContext&) = delete;
};
#endif
