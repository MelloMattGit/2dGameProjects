#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
//#include "multisprite.h"
//#include "twowaysprite.h"
#include "player.h"
#include "smartSprite.h"
#include "hud.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "dumbSprite.h"

Engine::~Engine() { 
  delete player;
  for (SmartSprite* ss : smartSprites) {
    delete ss;
  }
  for (DumbSprite* ds : dumbSprites) {
    delete ds;
  }

  delete collisionStrat;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("back", GameData::getInstance().getXmlInt("back/factor") ),
  sunset("sunset", GameData::getInstance().getXmlInt("sunset/factor")),
  backCloud("cloudback", GameData::getInstance().getXmlInt("cloudback/factor") ),
  foreCloud("cloudfore", GameData::getInstance().getXmlInt("cloudfore/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Pidgeot")),
  smartSprites(),
  dumbSprites(),
  collisionStrat(new MidpointCollisionStrategy),
  currentSprite(0),
  hud(Hud::getInstance() ),

  makeVideo( false ),
  collision(false)
{
  int n = GameData::getInstance().getXmlInt("bugNum");
  smartSprites.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i=0; i<n; i++) {
    smartSprites.emplace_back(new SmartSprite("bug", pos, w, h));
    player->attach(smartSprites[i]);
  }
  for (int i=0; i<n; i++) {
    dumbSprites.emplace_back(new DumbSprite("Fearow"));
  }

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  sunset.draw();
  backCloud.draw();
  foreCloud.draw();
  player->draw();
  for(const SmartSprite* s : smartSprites){
    s->SmartSprite::draw();
  }
  for(const DumbSprite* s : dumbSprites){
    s->DumbSprite::draw();
  }

  hud.draw();
  std::stringstream fps;
  fps << "FPS: " << clock.getFps();
  io.writeText(fps.str(), 690, 15 ) ;
  std::stringstream name;
  name << "Matt Hall";
  io.writeText(name.str(), 15, 760);
  viewport.draw();
  SDL_RenderPresent(renderer);

}

void Engine::update(Uint32 ticks) {
  player->update(ticks);
  for(SmartSprite* s : smartSprites) s->update(ticks);
  for(DumbSprite* s : dumbSprites) s ->update(ticks);
  sky.update();
  sunset.update();
  backCloud.update();
  foreCloud.update();
  viewport.update(); // always update viewport last
}


void Engine::checkForCollisions() {
  auto it = smartSprites.begin();
  while (it != smartSprites.end()) {
    if (collisionStrat->execute(*player, **it)) {
      collision = true;
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = smartSprites.erase(it);
    }
    else ++it;
  }
}

void Engine::switchSprite(){
  /*  if ( currentSprite == 1 ) {
    Viewport::getInstance().setObjectToTrack(ghastly);
  }
  else if (currentSprite == 2){
    Viewport::getInstance().setObjectToTrack(diglett);
  }
  else if (current Sprite == 3){
    Viewport::getInstance().setObjectToTrack(dugtrio);
  }
*/
  //Viewport::getInstance().setObjectToTrack(spriteVec[currentSprite++]);
 // if(spriteVec[currentSprite-1] == spriteVec.back()) currentSprite = 0;
  
}
void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  while (!done) {
    // The next loop polls for events, guarding against key bounce:
    while (SDL_PollEvent(&event)) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_P]) {
          if (clock.isPaused()) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          hud.setVisibility(!hud.isVisible());
        }
	if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }

      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->moveLeft();
        for (int i=0; i<100; i++) {
          IoMod::getInstance().writeText("pressing A", 50, 50);
        }
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->moveRight();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->moveUp();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->moveDown();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();

    	}
    }
}
}
