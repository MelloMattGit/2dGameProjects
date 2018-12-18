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

  enum MODE {NORMAL, EVADE};

  const Vector2f& getPlayerPos() const { return playerPos; }
  int getPlayerWidth() const           { return playerWidth; }
  int getPlayerHeight() const          { return playerHeight; }
  float getSafeDistance() const        { return safeDistance; }
  MODE getCurrentMode() const          { return currentMode; }
  bool hasCollided() const             { return collided; }
  bool isColliding() const { return colliding; }

  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  void setPlayerWidth(int w)           { playerWidth = w; }
  void setPlayerHeight(int h)          { playerHeight = h; }
  void setSafeDistance(float sd)       { safeDistance = sd; }
  void setCurrentMode(MODE m) { currentMode = m; }
  virtual void collide(int v);
  virtual void collide();
    void drop();
private:

  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;
  bool collided;
  bool colliding;
  DumbSprite* explosion;
    DumbSprite* bomb;
  unsigned int explosionStartTime;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();

};
#endif
