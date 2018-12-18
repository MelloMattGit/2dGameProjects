#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowaysprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() { 
  for(auto& sp : spriteVec) delete sp;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  sunset("sunset", Gamedata::getInstance().getXmlInt("sunset/factor")),
  backCloud("cloudback", Gamedata::getInstance().getXmlInt("cloudback/factor") ),
  foreCloud("cloudfore", Gamedata::getInstance().getXmlInt("cloudfore/factor") ),
  viewport( Viewport::getInstance() ),
  ghastly(new Sprite("Ghastly")),
  diglett(new MultiSprite("Diglett")),
  dugtrio(new MultiSprite("Dugtrio")),
  pidgeot(new TwoWaySprite("Pidgeot")),
  fearow(new TwoWaySprite("Fearow")),
  biker(new TwoWaySprite("Biker")), 
  metapod(new MultiSprite("Metapod")),
  currentSprite(0),
  spriteVec({ghastly, diglett, dugtrio, pidgeot, fearow, biker, metapod}),
  //spriteVec({new Sprite("Ghastly"), diglett, dugtrio, pidgeot, fearow, biker, metapod}),

  makeVideo( false )
{
  Viewport::getInstance().setObjectToTrack(ghastly);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  sunset.draw();
  backCloud.draw();
  foreCloud.draw();
  ghastly->draw();
  diglett->draw();
  dugtrio->draw();
  pidgeot->draw();
  fearow->draw();
  biker->draw();
  metapod->draw();
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
  ghastly->update(ticks);
  diglett->update(ticks);
  dugtrio->update(ticks);
  pidgeot->update(ticks);
  fearow->update(ticks);
  biker->update(ticks);
  metapod->update(ticks);
  sky.update();
  sunset.update();
  backCloud.update();
  foreCloud.update();
  viewport.update(); // always update viewport last
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
  Viewport::getInstance().setObjectToTrack(spriteVec[currentSprite++]);
  if(spriteVec[currentSprite-1] == spriteVec.back()) currentSprite = 0;
  
}
void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
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
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
