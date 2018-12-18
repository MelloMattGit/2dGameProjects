#include "gameData.h"
#include "player.h"
#include "smartSprite.h"

Player::Player(const std::string& name) :
  Sprite(name),
  images(RenderContext::getInstance()->getImages(name)),
  observers(),
  collision(false),
  startingVelocity(getVelocity()),
  slowDownFactor(GameData::getInstance().getXmlFloat(name+"/slowDownFactor"))
{ 
  setVelocityX(0); setVelocityY(0);
}

Player::Player(const Player& s) :
  Sprite(s),
  images(s.images),
  observers(s.observers),
  collision(s.collision),
  startingVelocity(s.getVelocity()),
  slowDownFactor(s.slowDownFactor)
{ setVelocityX(0); setVelocityY(0); }

Player& Player::operator= (const Player& s) {
  Sprite::operator=(s);
  observers = s.observers;
  collision = s.collision;
  startingVelocity = s.startingVelocity;
  slowDownFactor = s.slowDownFactor;
  return *this;
}

void Player::stop() {
  setVelocityX(slowDownFactor * getVelocityX());
  setVelocityY(slowDownFactor * getVelocityY());
}

void Player::moveRight() {
  if (getPositionX() < getMaxPosBoundaryX() - getScaledWidth()) {
    setVelocityX(startingVelocity[0]);
  }
}

void Player::moveLeft() {
  if (getPositionX() > getMinPosBoundaryX()) {
    setVelocityX(-startingVelocity[0]);
  }
}

void Player::moveUp() {
  if (getPositionY() > getMinPosBoundaryY()) {
    setVelocityY(-startingVelocity[1]);
  }
}

void Player::moveDown() {
  if (getPositionY() < getMaxPosBoundaryY() - getScaledHeight()) {
    setVelocityY(startingVelocity[1]);
  }
}

void Player::update(Uint32 ticks) {
  if (!collision) {
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

    setPosition(getPosition() + (getVelocity() * static_cast<float>(ticks) * 0.001));

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
  } else {
    std::cout << "COLLIDED!" << std::endl;
  }

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    (*ptr)->setPlayerPos(getPosition());
    ++ptr;
  }
  stop();
}

void Player::attach(SmartSprite* o) {
  observers.push_back(o);
}

void Player::detach(SmartSprite* o) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    if (*ptr == o) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
