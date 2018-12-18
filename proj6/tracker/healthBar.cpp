#include <sstream>
#include "healthBar.h"
#include "renderContext.h"

HealthBar& HealthBar::getInstance() {
  static HealthBar instance;
  return instance;
}

HealthBar::HealthBar() :
  renderer(RenderContext::getInstance()->getRenderer()),
  backBarWidth(GameData::getInstance().getXmlInt("healthBar/width")),
  backBarHeight(GameData::getInstance().getXmlInt("healthBar/height")),
  barColor({(Uint8)GameData::getInstance().getXmlInt("healthBar/barColor/r"),
            (Uint8)GameData::getInstance().getXmlInt("healthBar/barColor/g"),
            (Uint8)GameData::getInstance().getXmlInt("healthBar/barColor/b"),
            (Uint8)GameData::getInstance().getXmlInt("healthBar/barColor/a")}),
  backColor({(Uint8)GameData::getInstance().getXmlInt("healthBar/backColor/r"),
            (Uint8)GameData::getInstance().getXmlInt("healthBar/backColor/g"),
            (Uint8)GameData::getInstance().getXmlInt("healthBar/backColor/b"),
            (Uint8)GameData::getInstance().getXmlInt("healthBar/backColor/a")}),
  currentLength(1.0)
{ }

void HealthBar::draw(int playerX, int playerY, int playerWidth, int playerHeight) const {
  int viewWidth = GameData::getInstance().getXmlInt("view/width");
  int bgWidth = GameData::getInstance().getXmlInt("world/width");

  if ((playerX+(playerWidth/2)) > viewWidth / 2) {
    if (!(bgWidth - (playerX+(playerWidth/2)) < viewWidth / 2)) {
      playerX = (viewWidth / 2) - (playerWidth/2);
    } else {
      playerX = (viewWidth / 2) + (playerX-(1600));
    }
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
  SDL_Rect backBarRect = {playerX+10, playerY+playerHeight-10, backBarWidth, backBarHeight};
  SDL_RenderFillRect(renderer, &backBarRect);

  SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
  SDL_Rect healthBarRect = {playerX+12, playerY+playerHeight-8, (int)(currentLength * backBarWidth)-4, backBarHeight-4};
  SDL_RenderFillRect(renderer, &healthBarRect);
}
