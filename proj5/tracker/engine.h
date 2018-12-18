#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
//#include "drawable.h"

class Player;
class SmartSprite;
class DumbSprite;
class CollisionStrategy;
class Hud;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;
  SDL_Renderer * const renderer;
  World sky;
  World sunset;
  World backCloud;
  World foreCloud;
  Viewport& viewport;

  //Drawable* star;
  Player* player;
  std::vector<SmartSprite*> smartSprites;
  std::vector<DumbSprite*> dumbSprites;
  CollisionStrategy* collisionStrat;
  //Drawable* ghastly;
 // Drawable* diglett;
 // Drawable* dugtrio;
 // Drawable* pidgeot;
 // Drawable* fearow;
 // Drawable* biker;
  //Drawable* metapod;
  int currentSprite;
  Hud& hud;
  //std::vector <Drawable*> spriteVec;

  bool makeVideo;
  bool collision;
  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();


};
