#include <SDL_image.h>
#include <sstream>
#include <string>
#include <algorithm>
#include "gameStart.h"
#include "gameData.h"
#include "ioMod.h"
#include "dumbSprite.h"

GameStart& GameStart::getInstance() {
  static GameStart instance;
  return instance;
}

GameStart::GameStart() :
  width(GameData::getInstance().getXmlInt("gameStart/width")),
  height(GameData::getInstance().getXmlInt("gameStart/height")),
  pos(Vector2f(GameData::getInstance().getXmlInt("gameStart/position/x"),
               GameData::getInstance().getXmlInt("gameStart/position/y"))),
  visible(true),
  backgroundColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/backgroundColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/backgroundColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/backgroundColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/backgroundColor/a"))}),
  outlineColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/outlineColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/outlineColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/outlineColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/outlineColor/a"))}),
  textColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/textColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/textColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/textColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameStart/textColor/a"))})
{ }

void GameStart::draw() {
  if (visible) {
    SDL_Rect outer;
    outer.x = pos[0];
    outer.y = pos[1];
    outer.w = width;
    outer.h = height;

    SDL_Rect inner;
    inner.x = pos[0]+5;
    inner.y = pos[1]+5;
    inner.w = width-10;
    inner.h = height-10;

    SDL_Renderer* renderer = IoMod::getInstance().getRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    std::string textToWrite;

    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderFillRect(renderer, &outer);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &inner);
    textToWrite += "\n    ________________";
    textToWrite += "\n\n       Clear Out The ";
    textToWrite += "\n           Beedrill";
    textToWrite += "\n    ________________";
    textToWrite += "\n\n\n        Press ENTER\n            to start!";
    IoMod::getInstance().writeTextWrappedBold(textToWrite, pos[0]+45, pos[1], width, textColor);
  }
}
