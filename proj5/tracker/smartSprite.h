#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include <ctime>
#include "sprite.h"


class Player;
class DumbSprite;

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  SmartSprite& operator=(const SmartSprite&);
  virtual ~SmartSprite();
  using Sprite::draw;
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void collided();
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  bool isColliding() const {return collision;}

private:
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;
  bool collision;
  DumbSprite* explosion;
  clock_t explosionStartTime;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
