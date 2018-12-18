#ifndef PLAYER__H
#define PLAYER__H

#include <list>
#include <cmath>
#include "sprite.h"
//#include "projectile.h"
#include "balloon.h"

class SmartSprite;
class DumbSprite;

class Player : public Sprite {
public:
  Player(const std::string&);
  Player(const Player&);
  Player& operator= (const Player&);
  virtual ~Player() { }
  virtual void draw() const;

  virtual void update (Uint32 ticks);

  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
  void stop();

  void attach(SmartSprite* o);
  void detach(SmartSprite* o);

  void collide();

  void shoot();
  std::list<Projectile*> getActiveProjectiles() const { return activeProjectiles; }
  std::list<Projectile*> getFreeProjectiles() const { return freeProjectiles; }

  unsigned int getLivesLeft() const { return livesLeft; }
  unsigned int getInitNumLives() const { return initNumLives; }
  bool hasCollided() const { return collided; }
  bool isColliding() const { return colliding; }
  void setCollided(bool b) { collided = b; }
  void loseLife() { --livesLeft; }
  bool isGodMode() const { return godMode; }
  void toggleGodMode() { godMode = !godMode; }
  int getObservers() {return observers.size();}

private:
  bool facingRight;
  std::list<SmartSprite*> observers;
  SmartSprite* bloon;
  bool collision;
  bool collided;
  bool colliding;

  Vector2f startingVelocity;
  float slowDownFactor;
  DumbSprite* explosion;
  unsigned int explosionStartTime;
  std::string projectileName;
  std::list<Projectile*> activeProjectiles;
  std::list<Projectile*> freeProjectiles;
  unsigned int initNumLives;
  unsigned int livesLeft;
  bool godMode;

  float minSpeed;
  float projectileInterval;
  float timeSinceLastShot;
};
#endif
