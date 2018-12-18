#include <string>
#include "image.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  // ~World() { } // The image will be deleted by the FrameFactory
  void update();
  void draw() const;
  World& operator=(const World&);

private:
  Image* const image;
  int factor;
  unsigned worldWidth;
  unsigned imageWidth;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
};
