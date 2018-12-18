#include <SDL_image.h>
#include <sstream>
#include <string>
#include <algorithm>
#include "gameBoss.h"
#include "ioMod.h"
#include "dumbSprite.h"

GameBoss& GameBoss::getInstance() {
  static GameBoss instance;
  return instance;
}

GameBoss::GameBoss() :
  width(GameData::getInstance().getXmlInt("gameBoss/width")),
  height(GameData::getInstance().getXmlInt("gameBoss/height")),
  pos(Vector2f(GameData::getInstance().getXmlInt("gameBoss/position/x"),
               GameData::getInstance().getXmlInt("gameBoss/position/y"))),
  visible(0),
  backgroundColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/backgroundColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/backgroundColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/backgroundColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/backgroundColor/a"))}),
  outlineColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/outlineColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/outlineColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/outlineColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/outlineColor/a"))}),
  textColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/textColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/textColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/textColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameBoss/textColor/a"))})
{ }

void GameBoss::draw() {
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
    textToWrite += "\n\n          Take Out  ";
    textToWrite += "\n          Team Rocket!";
    textToWrite += "\n    ________________";
    textToWrite += "\n\n\n        Press ENTER\n            to start!";
    IoMod::getInstance().writeTextWrappedBold(textToWrite, pos[0]+45, pos[1], width, textColor);
  }
}
