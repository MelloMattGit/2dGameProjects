#ifndef SPRITE__H
#define SPRITE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include "vector2f.h"
#include "image.h"
#include "gameData.h"
#include "renderContext.h"

class Sprite {
public:
  Sprite(const std::string& n) :
    name(n),
    images(RenderContext::getInstance()->getImages(name)),
    scale(GameData::getInstance().getXmlInt(name+"/scale")),
    position(Vector2f(GameData::getInstance().getXmlInt(name+"/startPos/x"),
                      GameData::getInstance().getXmlInt(name+"/startPos/y"))),
    velocity(Vector2f(GameData::getInstance().getXmlInt(name+"/startVel/x"),
                      GameData::getInstance().getXmlInt(name+"/startVel/y"))),
    minPosBoundary(Vector2f(GameData::getInstance().getXmlInt(name+"/minPosBoundary/x"),
                            GameData::getInstance().getXmlInt(name+"/minPosBoundary/y"))),
    maxPosBoundary(Vector2f(GameData::getInstance().getXmlInt(name+"/maxPosBoundary/x"),
                            GameData::getInstance().getXmlInt(name+"/maxPosBoundary/y"))),
    numFrames(GameData::getInstance().getXmlInt(name+"/frames")),
    currentFrame(0),
    frameInterval(GameData::getInstance().getXmlInt(name+"/frameInterval")),
    timeSinceLastFrame(0),
    twoWay(GameData::getInstance().getXmlBool(name+"/twoWay"))
  { }

  Sprite(const Sprite& s):
    name(s.name),
    images(s.images),
    scale(s.scale),
    position(s.position),
    velocity(s.velocity),
    minPosBoundary(s.minPosBoundary),
    maxPosBoundary(s.maxPosBoundary),
    numFrames(s.numFrames),
    currentFrame(s.currentFrame),
    frameInterval(s.frameInterval),
    timeSinceLastFrame(s.timeSinceLastFrame),
    twoWay(s.twoWay)
  { }

  virtual ~Sprite() {}

  virtual void update(Uint32 ticks) = 0;
  virtual void draw() const { images[currentFrame]->draw(position[0], position[1], scale); }

  int getScaledWidth() const            { return getScale()*images[currentFrame]->getWidth(); }
  int getScaledHeight() const           { return getScale()*images[currentFrame]->getHeight(); }
  const SDL_Surface* getSurface() const { return images[currentFrame]->getSurface(); }
  const Image* getImage() const         { return images[currentFrame]; }
  float getScale() const                { return scale; }
  const std::string& getName() const    { return name; }
  const Vector2f& getVelocity() const   { return velocity; }
  const Vector2f& getPosition() const   { return position; }
  float getPositionX() const            { return position[0]; }
  float getPositionY() const            { return position[1]; }
  float getVelocityX() const            { return velocity[0]; }
  float getVelocityY() const            { return velocity[1]; }
  int getMinPosBoundaryX() const        { return minPosBoundary[0]; }
  int getMinPosBoundaryY() const        { return minPosBoundary[1]; }
  int getMaxPosBoundaryX() const        { return maxPosBoundary[0]; }
  int getMaxPosBoundaryY() const        { return maxPosBoundary[1]; }
  float getTimeSinceLastFrame() const   { return timeSinceLastFrame; }
  unsigned getCurrentFrame() const      { return currentFrame; }
  unsigned getNumFrames() const         { return numFrames; }
  unsigned getFrameInterval() const     { return frameInterval; }
  bool isTwoWay() const                 { return twoWay; }

  void setScale(float s)                { scale = s; }
  void setName(const std::string& n)    { name = n; }
  void setVelocity(const Vector2f& vel) { velocity = vel; }
  void setPosition(const Vector2f& pos) { position = pos; }
  void setPositionX(float x)            { position[0] = x; }
  void setPositionY(float y)            { position[1] = y; }
  void setVelocityX(float vx)           { velocity[0] = vx; }
  void setVelocityY(float vy)           { velocity[1] = vy; }
  void setMinPosBoundaryX(float x)      { minPosBoundary[0] = x; }
  void setMinPosBoundaryY(float y)      { minPosBoundary[1] = y; }
  void setMaxPosBoundaryX(float x)      { maxPosBoundary[0] = x; }
  void setMaxPosBoundaryY(float y)      { maxPosBoundary[1] = y; }
  void setTimeSinceLastFrame(float t)   { timeSinceLastFrame = t; }
  void setCurrentFrame(unsigned f)      { currentFrame = f; }

private:
  std::string name;
  std::vector<Image *> images;
  float scale;
  Vector2f position;
  Vector2f velocity;
  Vector2f minPosBoundary;
  Vector2f maxPosBoundary;
  unsigned numFrames;
  unsigned currentFrame;
  unsigned frameInterval;
  float timeSinceLastFrame;
  bool twoWay;
};
#endif

