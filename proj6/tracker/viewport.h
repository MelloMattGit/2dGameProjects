#ifndef VIEWPORT__H
#define VIEWPORT__H
#include "sprite.h"
#include "gameData.h"

class Viewport {
public:
  static Viewport& getInstance();
  void draw() const;
  void update();

  Vector2f getPosition() const { return viewPos; }
  float getX() const  { return viewPos[0]; }
  void  setX(float x) { viewPos[0] = x; }
  float getY() const  { return viewPos[1]; }
  void  setY(float y) { viewPos[1] = y; }

  void setObjectToTrack(const Sprite *obj);
  const Sprite* getObjectToTrack() const { return objectToTrack; } 

private:
  const GameData& gdata;
  Vector2f viewPos;
  Vector2f msgPos;
  int worldWidth;
  int worldHeight;
  int viewWidth;
  int viewHeight;
  int objWidth;
  int objHeight;
  
  const Sprite *objectToTrack;

  Viewport();
  Viewport(const Viewport&);
  Viewport& operator=(const Viewport&);
};
#endif
