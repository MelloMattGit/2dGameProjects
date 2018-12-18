#ifndef 2WAYSPRITE__H
#define 2WAYSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class 2waySprite : public Drawable {
public:
  2waySprite(const std::string&);
  2waySprite(const 2waySprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  2waySprite& operator=(const 2waySprite&);
};
#endif
