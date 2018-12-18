#include <SDL_image.h>
#include "ioMod.h"
#include "gameData.h"
#include "renderContext.h"

IoMod& IoMod::getInstance() {
  static IoMod instance;
  return instance;
}

IoMod::~IoMod() { 
  TTF_CloseFont(font);
  TTF_Quit(); 
}

IoMod::IoMod() : 
  init(TTF_Init()),
  renderer( RenderContext::getInstance()->getRenderer() ),
  font(TTF_OpenFont(GameData::getInstance().getXmlStr("font/file").c_str(),
                    GameData::getInstance().getXmlInt("font/size"))),
  textColor({0xff, 0, 0, 0})
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  textColor.r = GameData::getInstance().getXmlInt("font/red");
  textColor.g = GameData::getInstance().getXmlInt("font/green");
  textColor.b = GameData::getInstance().getXmlInt("font/blue");
  textColor.a = GameData::getInstance().getXmlInt("font/alpha");
}

SDL_Texture* IoMod::readTexture(const std::string& filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename.c_str());
  if ( texture == NULL ) {
    throw std::string("Couldn't load ") + filename;
  }
  return texture;
}

SDL_Surface* IoMod::readSurface(const std::string& filename) {
  SDL_Surface *surface = IMG_Load(filename.c_str());
  if ( !surface ) {
    throw std::string("Couldn't load ") + filename;
  }
  return surface;
}

void IoMod::writeText(const std::string& msg, int x, int y) const {
  if(true){
	writeText(msg, x, y, SDL_Color ({255,0,0,0}));
	return;
  }

  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IoMod::writeText(const std::string& msg, int x, int y, SDL_Color fontColor) const{
  SDL_Surface* surface = 
    TTF_RenderText_Solid(font, msg.c_str(), fontColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);

}


void IoMod::writeTextWrapped(const std::string& msg, int x, int y, int wrapWidth) const {
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, msg.c_str(), textColor, wrapWidth);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  int textWidth = surface->w;
  int textHeight = surface->h;

  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};
  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void IoMod::writeTextWrapped(const std::string& msg, int x, int y, int wrapWidth, SDL_Color customColor) const {
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, msg.c_str(), customColor, wrapWidth);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  int textWidth = surface->w;
  int textHeight = surface->h;

  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};
  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
