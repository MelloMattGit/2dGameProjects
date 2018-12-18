#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"
#include "dumbSprite.h"
#include "sound.h"
#include "clock.h"

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
  safeDistance(GameData::getInstance().getXmlFloat(name + "/safeDistance")),
  collided(false),
  colliding(false),
  explosion(nullptr),
  bomb(nullptr),
  explosionStartTime(-1)
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
  safeDistance(s.safeDistance),
  collided(s.collided),
  colliding(s.colliding),
  explosion(s.explosion),
  bomb(s.bomb),
  explosionStartTime(s.explosionStartTime)
{}
void SmartSprite::draw() const{
 // if(colliding) explosion -> draw();
   getImage()->draw(getPositionX(), getPositionY(), getScale());

}

void SmartSprite::update(Uint32 ticks) {
  if (colliding) {
  //  explosion->update(ticks);
 if ((Clock::getInstance().getSeconds() - explosionStartTime) >= 2) {
      colliding = false;
      delete explosion;
      explosion = NULL;
    }
    return;
  } else {
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
  float x = getPositionX() + getImage()->getWidth()/2;
  float y = getPositionY() + getImage()->getHeight()/2;
  float ex = playerPos[0] + playerWidth/2;
  float ey = playerPos[1] + playerHeight/2;
  float distanceToEnemy = ::distance(x, y, ex, ey);
  if (getCurrentMode() == NORMAL) {
    if (distanceToEnemy < safeDistance) {
      setCurrentMode(EVADE);
    }
  } else if (getCurrentMode() == EVADE) {
    if (distanceToEnemy > safeDistance) {
      setCurrentMode(NORMAL);
  } else {
      if (x < ex) goLeft();
      if (x > ex) goRight();
      if (y < ey) goUp();
      if (y > ey) goDown();
    }
  }
}
}

void SmartSprite::collide(int v) {
  SDL_Sound::getInstance()[1];
  collided = true;
  colliding = true;
  setVelocityX(2*v);
  setVelocityY(200);
  setMinPosBoundaryX(-10);
  setMinPosBoundaryY(-10);
  setMaxPosBoundaryX(3000);
 // explosion = new DumbSprite("Explosion");
 // explosion->setPosition(getPosition());
 // explosion->setVelocityX(0);
//  explosion->setVelocityY(0);
//  setPosition(Vector2f(-100, -100));
//  setVelocity(Vector2f(0, 0));
 // explosionStartTime = Clock::getInstance().getSeconds();
}

void SmartSprite::collide() {
  SDL_Sound::getInstance()[0];
  collided = true;
  colliding = true;
  setVelocityX(0);
  setVelocityY(0);
  //setMinPosBoundaryX(-10);
  //setMinPosBoundaryY(-10);
  //setMaxPosBoundaryX(3000);
  explosion = new DumbSprite("Explosion");
 explosion->setPosition(getPosition());
 explosion->setVelocityX(0);
  explosion->setVelocityY(0);
  setPosition(Vector2f(-100, -100));
  setVelocity(Vector2f(0, 0));
 explosionStartTime = Clock::getInstance().getSeconds();
}

void SmartSprite::drop(){
  if (getPositionY() < playerPos[1] && getPositionX() == playerPos[0]){
    bomb = new DumbSprite("missile");
    bomb->setPosition(getPosition());
    bomb->setVelocityX(0);
    bomb->setVelocityY(-200);

  }
}
