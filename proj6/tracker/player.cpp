#include "gameData.h"
#include "player.h"
#include "smartSprite.h"
#include "clock.h"
#include "sound.h"

Player::Player(const std::string& name) :
  Sprite(name),
  facingRight(true),
  observers(),
  bloon(),
  collision(false),
  collided(false),
  colliding(false),
  startingVelocity(getVelocity()),
  slowDownFactor(GameData::getInstance().getXmlFloat(name+"/slowDownFactor")),
  explosion(nullptr),
  explosionStartTime(-1),
  projectileName(GameData::getInstance().getXmlStr(name+"/projectileName")),
  activeProjectiles(),
  freeProjectiles(),
  initNumLives(GameData::getInstance().getXmlInt("numLives")),
  livesLeft(GameData::getInstance().getXmlInt("numLives")),
  godMode(false),
  minSpeed(GameData::getInstance().getXmlInt(projectileName+"/minSpeed")),
  projectileInterval(GameData::getInstance().getXmlInt(projectileName+"/interval")),
timeSinceLastShot(4294967295)
{
  setVelocityX(0); setVelocityY(0);
}

Player::Player(const Player& s) :
  Sprite(s),
  facingRight(s.facingRight),
  observers(s.observers),
  bloon(s.bloon),
  collision(s.collision),
  collided(s.collided),
  colliding(s.colliding),
  startingVelocity(s.getVelocity()),
  slowDownFactor(s.slowDownFactor),
  explosion(s.explosion),
  explosionStartTime(s.explosionStartTime),
  projectileName(s.projectileName),
  activeProjectiles(s.activeProjectiles),
  freeProjectiles(s.freeProjectiles),
  initNumLives(s.initNumLives),
  livesLeft(s.livesLeft),
  godMode(s.godMode),
  minSpeed(s.minSpeed),
  projectileInterval(s.projectileInterval),
  timeSinceLastShot(s.timeSinceLastShot)
{ setVelocityX(0); setVelocityY(0); }

Player& Player::operator= (const Player& s) {
  Sprite::operator=(s);
  facingRight = s.facingRight;
  observers = s.observers;
  bloon = s.bloon;
  collision = s.collision;
  startingVelocity = s.startingVelocity;
  slowDownFactor = s.slowDownFactor;
  explosion = s.explosion;
  projectileName = s.projectileName;
  activeProjectiles = s.activeProjectiles;
  freeProjectiles = s.freeProjectiles;
  minSpeed = s.minSpeed;
  projectileInterval = s.projectileInterval;
  timeSinceLastShot = s.timeSinceLastShot;
  return *this;
}

void Player::draw() const{
  if (colliding) explosion -> draw();
  else Sprite::draw();
  for(Projectile* p : activeProjectiles){
     p->draw();
  }
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
  if(livesLeft <= 0){
    setPositionY(-300);
  }

  if (collided && colliding) {
    explosion->update(ticks);
    activeProjectiles.clear();
    if ((Clock::getInstance().getSeconds() - explosionStartTime)  >= 2) {
      colliding = false;
      delete explosion;
      explosion = NULL;
    }
    return;
  } else {
    setTimeSinceLastFrame(getTimeSinceLastFrame() + ticks);
    timeSinceLastShot += ticks;
    if (getTimeSinceLastFrame() > getFrameInterval()) {
      if (isTwoWay()) {
        if (getVelocityX() > 0 || (getVelocityX() == 0 && facingRight)) {
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
  }

  auto it = activeProjectiles.begin();
  while (it != activeProjectiles.end()) {
    (*it)->update(ticks);
    if ((*it)->isTooFar()) {
      freeProjectiles.push_back(*it);
      it = activeProjectiles.erase(it);
    }
    it++;
  }
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    (*ptr)->setPlayerPos(getPosition());
    ++ptr;
  }
  //bloon->setPlayerPos(getPosition());

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

void Player::collide() {

if (!godMode ){
  SDL_Sound::getInstance()[2];
  collided = true;
  colliding = true;
  explosion = new DumbSprite("Hurt");
  explosion->setPosition(getPosition());
  explosion->setVelocityX(0);
  explosion->setVelocityY(0);
  explosionStartTime = Clock::getInstance().getSeconds();
}
}

void Player::shoot() {
  if (!collision) {
    if (timeSinceLastShot < projectileInterval) return;
    float x = getScaledWidth();
    float y = getScaledHeight()/6;

    // object pooling
    if (freeProjectiles.empty()) {
      Projectile *p = new Projectile(projectileName);
      activeProjectiles.push_back(p);
      if (getVelocityX() > 0 || (getVelocityX() == 0 && facingRight)) {
        activeProjectiles.back()->setPosition(getPosition() + Vector2f(3*x/4, y));
        activeProjectiles.back()->setStartingPos(activeProjectiles.back()->getPosition());
        activeProjectiles.back()->setVelocity(getVelocity() + Vector2f(minSpeed, 0));
      } else {
        activeProjectiles.back()->setPosition(getPosition() + Vector2f(-x/4, y));
        activeProjectiles.back()->setStartingPos(activeProjectiles.back()->getPosition());
        activeProjectiles.back()->setVelocity(getVelocity() + Vector2f(-minSpeed, 0));
      }
    } else {
      Projectile *p = freeProjectiles.front();
      freeProjectiles.pop_front();
      p->reset();
      if (getVelocityX() > 0 || (getVelocityX() == 0 && facingRight)) {
        p->setPosition(getPosition()+Vector2f(3*x/4, y));
        p->setStartingPos(p->getPosition());
        p->setVelocity(getVelocity()+Vector2f(minSpeed,0));
      } else {
        p->setPosition(getPosition()+Vector2f(-x/4, y));
        p->setStartingPos(p->getPosition());
        p->setVelocity(getVelocity()+Vector2f(-minSpeed,0));
      }
      activeProjectiles.push_back(p);
    }
    timeSinceLastShot = 0;
  }
}
