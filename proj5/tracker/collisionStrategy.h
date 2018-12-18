#include <cmath>
#include "sprite.h"

class CollisionStrategy {
public:
  virtual bool execute(const Sprite&, const Sprite&) const = 0;
  virtual ~CollisionStrategy() { }
};

class MidpointCollisionStrategy : public CollisionStrategy {
public:
  MidpointCollisionStrategy() {}
  virtual bool execute(const Sprite& s1, const Sprite& s2) const {
    int width1 = s1.getScaledWidth();
    int width2 = s2.getScaledWidth();
    int height1 = s1.getScaledHeight();
    int height2 = s2.getScaledHeight();

    int collisionDistance = width1/2 + width2/2;
    float x1 = s1.getPositionX() + width1/2;
    float y1 = s1.getPositionY() + height1/2;
    float x2 = s2.getPositionX() + width2/2;
    float y2 = s2.getPositionY() + height2/2;
    return hypot((x1-x2), (y1-y2)) < collisionDistance;
  };
};
