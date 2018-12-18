#include <iostream>
#include "sprite.h"
#include "gameData.h"

class Bullet : public Sprite {
public:
  explicit Bullet(const string& name) :
    Sprite(name), 
    distance(0), 
    maxDistance(GameData::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
