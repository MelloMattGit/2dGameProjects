#include <vector>
#include <SDL.h>
//#include "ioMod.h"
#include "renderContext.h"
//#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "sound.h"
#include "healthBar.h"
#include "gameStart.h"
#include "gameOver.h"
#include "gameBoss.h"
#include "hud.h"
#include "menuEngine.h"



class Player;
class SmartSprite;
class DumbSprite;
//class Balloon;
class CollisionStrategy;
class Hud;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;
  HealthBar& healthBar;
  GameStart& gameStart;
  GameBoss& gameBoss;
  GameOver& gameOver;
  SDL_Renderer * const renderer;
  World back;
  World sunset;
  World backCloud;
  World foreCloud;
  Viewport& viewport;
  SDL_Sound& sound;
  //SmartSprite* balloon;
  std::vector<SmartSprite*> smartSprites;
  std::vector<DumbSprite*> dumbSprites;
  CollisionStrategy* collisionStrat;
  Player* player;
  int currentSprite;
  Hud& hud;
  //MenuEngine menuEngine;
  //std::vector <Drawable*> spriteVec;

  bool makeVideo;
  bool collision;
  void draw(int) const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
  bool stage2();

};
