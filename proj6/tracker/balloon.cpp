#include <cmath>
#include <random>
#include <functional>
#include "balloon.h"
#include "gameData.h"
#include "renderContext.h"
#include "sound.h"
#include "clock.h"
Balloon::~Balloon(){}

Balloon::Balloon(const std::string& name, Vector2f pos, int w, int h) :
  Sprite(name),
  playerPos(pos),
  playW(w),
  playH(h),
  health(5),
  collision(false),
  collided(false),
  colliding(false),
  explosion(nullptr),
  explosionStartTime(-1),
  projectileName(GameData::getInstance().getXmlStr(name+"/projectileName")),
  projectileInterval(GameData::getInstance().getXmlInt(projectileName+"/interval")),
  timeSinceLastShot(5000000),
  activeProjectiles(),
  freeProjectiles()
{
  setPositionX(GameData::getInstance().getXmlInt("balloon/startPos/x"));
  setPositionY(GameData::getInstance().getXmlInt("balloon/startPos/y"));
}

Balloon::Balloon(const Balloon& s) :
  Sprite(s),
  playerPos(s.playerPos),
  playW(s.playW),
  playH(s.playH),
  health(s.health),
  collision(s.collision),
  collided(s.collided),
  colliding(s.colliding),
  explosion(s.explosion),
  explosionStartTime(s.explosionStartTime),
  projectileName(s.projectileName),
  projectileInterval(s.projectileInterval),
  timeSinceLastShot(s.timeSinceLastShot),
  activeProjectiles(s.activeProjectiles),
  freeProjectiles(s.freeProjectiles)
{}
void Balloon::draw() const{
  if (colliding) explosion -> draw();
  //Clock::getInstance().pause();}
  else getImage()->draw(getPositionX(), getPositionY(), getScale());
  //std::cout<<"TEST"<<std::endl;
}

void Balloon::update(Uint32 ticks) {
  if(health <= 0){
    setPositionY(-500);
  }
  if (collided && colliding) {
    explosion->update(ticks);
  if ((Clock::getInstance().getSeconds() - explosionStartTime)  >= 2) {
      colliding = false;
      delete explosion;
      explosion = NULL;

    }
    return;
  }
  else{
  timeSinceLastShot += ticks;
  //shoot();
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
}

void Balloon::collide() {
  loseHealth();
  if(health <=0){
  SDL_Sound::getInstance()[0];
  collided = true;
  colliding = true;
  explosion = new DumbSprite("Explosion");
  explosion->setPositionX(800);
  explosion->setPositionY(200);
  explosion->setVelocityX(0);
  explosion->setVelocityY(0);
  explosionStartTime = Clock::getInstance().getSeconds();
  }
}

void Balloon::shoot(){
  if (!collision) {
    if (timeSinceLastShot < projectileInterval ) return;
    float x = getScaledWidth();
    float y = getScaledHeight()/6;

    // object pooling
    if (freeProjectiles.empty()) {
      Projectile *p = new Projectile(projectileName);
      activeProjectiles.push_back(p);
      std::cout<<"TEST";
        activeProjectiles.back()->setPosition(getPosition() + Vector2f(x/2, 6*y/7));
        activeProjectiles.back()->setStartingPos(activeProjectiles.back()->getPosition());
        activeProjectiles.back()->setVelocity(Vector2f(getPositionX()-playerPos[0], getPositionY()-playerPos[1]));

    } else {
      Projectile *p = freeProjectiles.front();
      freeProjectiles.pop_front();
      p->reset();

        p->setPosition(getPosition()+Vector2f(3*x/4, y));
        p->setStartingPos(p->getPosition());
        p->setVelocity(getVelocity()+Vector2f(getPositionX()-playerPos[0], getPositionY()-playerPos[1]));

      activeProjectiles.push_back(p);
    }
    timeSinceLastShot = 0;
  }
  }
