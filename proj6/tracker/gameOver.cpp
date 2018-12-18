#include <SDL_image.h>
#include <sstream>
#include <string>
#include <algorithm>
#include "gameOver.h"
#include "gameData.h"
#include "ioMod.h"
#include "dumbSprite.h"

GameOver& GameOver::getInstance() {
  static GameOver instance;
  return instance;
}

GameOver::GameOver() :
  width(GameData::getInstance().getXmlInt("gameOver/width")),
  height(GameData::getInstance().getXmlInt("gameOver/height")),
  pos(Vector2f(GameData::getInstance().getXmlInt("gameOver/position/x"),
               GameData::getInstance().getXmlInt("gameOver/position/y"))),
  visible(false),
  winBackgroundColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/backgroundColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/backgroundColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/backgroundColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/backgroundColor/a"))}),
  winOutlineColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/outlineColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/outlineColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/outlineColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/outlineColor/a"))}),
  winTextColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/textColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/textColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/textColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/win/textColor/a"))}),
  loseBackgroundColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/backgroundColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/backgroundColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/backgroundColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/backgroundColor/a"))}),
  loseOutlineColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/outlineColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/outlineColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/outlineColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/outlineColor/a"))}),
  loseTextColor({static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/textColor/r")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/textColor/g")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/textColor/b")),
                      static_cast<Uint8>(GameData::getInstance().getXmlInt("gameOver/lose/textColor/a"))})
{ }

void GameOver::draw(bool won) {
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

    if (won) {
      SDL_SetRenderDrawColor(renderer, winOutlineColor.r, winOutlineColor.g, winOutlineColor.b, winOutlineColor.a);
      SDL_RenderFillRect(renderer, &outer);
      SDL_SetRenderDrawColor(renderer, winBackgroundColor.r, winBackgroundColor.g, winBackgroundColor.b, winBackgroundColor.a);
      SDL_RenderFillRect(renderer, &inner);
      textToWrite += "\n\n\n\n          YOU WIN!";
      textToWrite += "\n\n\n Press R to play again!";
      IoMod::getInstance().writeTextWrappedBold(textToWrite, pos[0]+45,pos[1], width, winTextColor);
    } else {
      SDL_SetRenderDrawColor(renderer, loseOutlineColor.r, loseOutlineColor.g, loseOutlineColor.b, loseOutlineColor.a);
      SDL_RenderFillRect(renderer, &outer);
      SDL_SetRenderDrawColor(renderer, loseBackgroundColor.r, loseBackgroundColor.g, loseBackgroundColor.b, loseBackgroundColor.a);
      SDL_RenderFillRect(renderer, &inner);
      textToWrite += "\n\n\n\n         YOU LOSE!";
      textToWrite += "\n\n\n   Press R to try again!";
      IoMod::getInstance().writeTextWrappedBold(textToWrite, pos[0]+45, pos[1], width, loseTextColor);
    }


  }
}
