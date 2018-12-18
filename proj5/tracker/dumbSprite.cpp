#include <cmath>
#include <random>
#include <functional>
#include "dumbSprite.h"
#include "gameData.h"
#include "renderContext.h"


DumbSprite::~DumbSprite(){}

DumbSprite::DumbSprite(const std::string& name) :
  Sprite(name)
{
  setPositionX(GameData::getInstance().getRandFloat(0, getMaxPosBoundaryX()));
  setPositionY(GameData::getInstance().getRandFloat(0, getMaxPosBoundaryY()));
}

DumbSprite::DumbSprite(const DumbSprite& s) :
  Sprite(s)
{}
void DumbSprite::draw() const{
  getImage()->draw(getPositionX(), getPositionY(), getScale()); 
}

void DumbSprite::update(Uint32 ticks) {
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
}



