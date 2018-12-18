#ifndef BALLOON_H
#define BALLOON__H
#include <list>
#include <string>
#include "sprite.h"
//#include "dumbSprite.h"
//#include "clock.h"
#include "vector2f.h"
#include "projectile.h"


class Balloon : public Sprite {
public:
  Balloon(const std::string&, Vector2f, int, int);
  Balloon(const Balloon&);
  Balloon& operator=(const Balloon&);
  virtual ~Balloon();
  using Sprite::draw;
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void loseHealth() {health--;}
  int getHealth(){return health;}
  void collide();
  std::list<Projectile*> getActiveProjectiles() const { return activeProjectiles; }
  std::list<Projectile*> getFreeProjectiles() const { return freeProjectiles; }
  void shoot();
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  void setDims(int a , int b ){
    playW = a;
    playH = b;
    }
private:
  Vector2f playerPos;
  int playW;
  int playH;
  int health;
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  bool collision;
  bool collided;
  bool colliding;
  DumbSprite* explosion;
  unsigned int explosionStartTime;
  std::string projectileName;
  float projectileInterval;
  float timeSinceLastShot;
  std::list<Projectile*> activeProjectiles;
  std::list<Projectile*> freeProjectiles;

};
#endif
