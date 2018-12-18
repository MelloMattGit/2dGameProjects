#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::~SmartSprite(){}

void SmartSprite::goLeft()  { setVelocityX( -std::abs(getVelocityX()) );  }
void SmartSprite::goRight() { setVelocityX( std::fabs(getVelocityX()) );  }
void SmartSprite::goUp()    { setVelocityY( -std::fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( std::fabs(getVelocityY()) );  }

SmartSprite& SmartSprite::operator=(const SmartSprite& s){
  std::cout << s.playerWidth << std::endl;
  return *this;
}

SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, 
  int w, int h) :
  Sprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(GameData::getInstance().getXmlFloat(name + "/safeDistance"))
{
  setPositionX(GameData::getInstance().getRandFloat(0, getMaxPosBoundaryX()));
  setPositionY(GameData::getInstance().getRandFloat(0, getMaxPosBoundaryY()));
}

SmartSprite::SmartSprite(const SmartSprite& s) :
  Sprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance)
{}
void SmartSprite::draw() const{
  getImage()->draw(getPositionX(), getPositionY(), getScale()); 
}

void SmartSprite::update(Uint32 ticks) {
  setTimeSinceLastFrame(getTimeSinceLastFrame() + ticks);
  if (getTimeSinceLastFrame() > getFrameInterval()) {
    if (isTwoWay()) {
      if (getVelocityX() <= 0) {
        setCurrentFrame(((getCurrentFrame()+1) % (getNumFrames()/2)));
      } else {
        setCurrentFrame((getNumFrames()/2) + ((getCurrentFrame()+1) % (getNumFrames()/2)));
      }
    } else {
      setCurrentFrame((getCurrentFrame()+1) % getNumFrames());
    }
    setTimeSinceLastFrame(0);
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if (getPositionY() < getMinPosBoundaryY()) {
    setVelocityY(std::abs(getVelocityY()));
  }
  if (getPositionY() > getMaxPosBoundaryY() - getScaledHeight()) {
    setVelocityY(-std::abs(getVelocityY()));
  }
  if ( getPositionX() < getMinPosBoundaryX()) {
    setVelocityX(std::abs(getVelocityX()));
  }
  if ( getPositionX() > getMaxPosBoundaryX() - getScaledWidth()) {
    setVelocityX(-std::abs(getVelocityX()));
  }

  float x = getPositionX() + getImage()->getWidth()/2;
  float y = getPositionY() + getImage()->getHeight()/2;
  float ex = playerPos[0] + playerWidth/2;
  float ey = playerPos[1] + playerHeight/2;
  float distanceToEnemy = ::distance(x, y, ex, ey);
  if (currentMode == NORMAL) {
    if (distanceToEnemy < safeDistance) {
      currentMode = EVADE;
    }
  } else if (currentMode == EVADE) {
    if (distanceToEnemy > safeDistance) {
      currentMode=NORMAL;  
  } else {
      if (x < ex) goLeft();
      if (x > ex) goRight();
      if (y < ey) goUp();
      if (y > ey) goDown();
    }
  }
}
