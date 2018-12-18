#ifndef DUMBSPRITE__H
#define DUMBSPRITE__H
#include <string>
#include "sprite.h"


class DumbSprite : public Sprite {
public:
  DumbSprite(const std::string&);
  DumbSprite(const DumbSprite&);
  DumbSprite& operator=(const DumbSprite&);
  virtual ~DumbSprite();
  using Sprite::draw;
  virtual void draw() const;
  virtual void update(Uint32 ticks);

private:

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
