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
#include "gameData.h"
#include "engine.h"
//#include "balloon.h"
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
  //delete balloon;
  delete collisionStrat;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  healthBar(HealthBar::getInstance()),
  gameStart(GameStart::getInstance()),
  gameBoss(GameBoss::getInstance()),
  gameOver(GameOver::getInstance()),
  renderer( rc->getRenderer() ),
  back("back", GameData::getInstance().getXmlInt("back/factor") ),
  sunset("sunset", GameData::getInstance().getXmlInt("sunset/factor")),
  backCloud("cloudback", GameData::getInstance().getXmlInt("cloudback/factor") ),
  foreCloud("cloudfore", GameData::getInstance().getXmlInt("cloudfore/factor") ),
  viewport( Viewport::getInstance() ),
  sound(SDL_Sound::getInstance()),
//  balloon(new SmartSprite("balloon", Vector2f(0,0), 0, 0)),
  smartSprites(),
  dumbSprites(),
  collisionStrat(new MidpointCollisionStrategy),
  player(new Player("Pidgeot")),
  currentSprite(0),
  hud(Hud::getInstance() ),
  //menuEngine(),
  makeVideo( false ),
  collision(false)
{


  int n = GameData::getInstance().getXmlInt("bugNum");
  smartSprites.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  //balloon = new SmartSprite("balloon",pos,w,h);
  for (int i=0; i<n; i++) {
    smartSprites.emplace_back(new SmartSprite("bug", pos, w, h));
    player->attach(smartSprites[i]);
  }
/*
  for (int i=0; i<n; i++) {
    dumbSprites.emplace_back(new DumbSprite("Fearow"));
  }
*/
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw(int boss) const {
  back.draw();
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
//  balloon->draw();
  hud.draw(player->getObservers());
  healthBar.draw(player->getPositionX(), player->getPositionY(), player->getScaledWidth(), player->getScaledHeight());
  std::stringstream fps;
  fps << "FPS: " << clock.getFps();
  io.writeText(fps.str(), 690, 15 ) ;
  std::stringstream name;
  name << "Matt Hall";
  io.writeText(name.str(), 15, 760);
  gameStart.draw();
  gameBoss.draw();
  viewport.draw();
  if (player->getLivesLeft() <= 0) {
    hud.setVisibility(false);
    gameOver.setVisibility(true);
    gameOver.draw(false);

  }
  if (boss == 1){
    //balloon->draw();
  }
  if(smartSprites.size() <= 0){
    hud.setVisibility(false);
    gameOver.setVisibility(true);
    gameOver.draw(true);
}


  SDL_RenderPresent(renderer);

}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  player->update(ticks);
  for(SmartSprite* s : smartSprites) s->update(ticks);
  for(DumbSprite* s : dumbSprites) s ->update(ticks);
//  balloon->update(ticks);
  back.update();
  sunset.update();
  backCloud.update();
  foreCloud.update();
  //balloon->update(ticks);
  //balloon->drop();
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {
  auto smartIt = smartSprites.begin();
  while (smartIt != smartSprites.end()) {
    if (!(player->getActiveProjectiles().empty())) {
      for (auto proj : player->getActiveProjectiles()) {
        if (collisionStrat->execute(*proj, **smartIt)) {
          (*smartIt)->collide((*proj).getVelocityX());
          (*proj).collide();
        }

        }
      }
    if ((collisionStrat->execute(*player, **smartIt)) && !(player->isColliding())) {
      player->collide();
      (*smartIt)->collide();
    }
    if (player->hasCollided() && !(player->isColliding())) {
      player->loseLife();
      healthBar.setCurrentLength(healthBar.getCurrentLength() - (1.0 / player->getInitNumLives()));
      player->setCollided(false);
    }
    if ((*smartIt)->hasCollided() && (!((*smartIt)->isColliding())) && ((*smartIt)->getPositionX() < 0 || (*smartIt)->getPositionX() > 2000  || (*smartIt)->getPositionY() < 0)) {
      SmartSprite* deadSmartSprite = *smartIt;
      player->detach(deadSmartSprite);
      smartIt = smartSprites.erase(smartIt);
      delete deadSmartSprite;
    } else {
      ++smartIt;
    }

  }
//  for(auto proj : player->getActiveProjectiles()){
    //if(collisionStrat->execute(*proj,*balloon)){
      //balloon->collide();
    //  (*proj).collide();
//    }

//  }
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
bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  draw(0);
  clock.pause();
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
        if (keystate[SDL_SCANCODE_RETURN]) {
          if (gameStart.isVisible()) {
            gameStart.setVisibility(false);
            hud.setVisibility(true);
	    clock.unpause();
          }
	}
        if ( keystate[SDL_SCANCODE_P] || keystate[SDL_SCANCODE_O] ) {
          clock.pause();
          //menuEngine.play();
          clock.unpause();
        }
        if (keystate[SDL_SCANCODE_M]) {
          sound.toggleMusic();
	}
        if (keystate[SDL_SCANCODE_T]) {
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
        if (keystate[SDL_SCANCODE_SPACE]) {
          player->shoot();
	}
        if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          //makeVideo = false;
        }
	if (keystate[SDL_SCANCODE_R]) {
          //clock.unpause();
          return true;
        }
        if (keystate[SDL_SCANCODE_G]) {
          player->toggleGodMode();
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

/*	if(smartSprites.size() <= 3) {
   	 hud.setVisibility(false);
     gameBoss.setVisibility(true);
     gameBoss.draw();
   	 stage2();
     std::cout<<"TEST"<<std::endl;
     draw();

	}*/


      draw(0);
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();

    	}

    }
    if(smartSprites.size() <= 0){
    //  gameOver(true);
    //  bool end = stage2();
    //  draw();
      hud.setVisibility(false);
      gameOver.setVisibility(true);
      gameOver.draw(true);
    }


  //  if (end == true){return false;}
  //  else return true;
  //}
}
  return false;
}

bool Engine::stage2(){

  std::string a = "back";
  std::string b = "sky";

//  back(a, GameData::getInstance().getXmlInt("back2/factor"));
//	sunset(b, GameData::getInstance().getXmlInt("sky/factor"));
  player->setPositionX(100);
  player->setPositionY(300);
	//gameBoss.setVisibility(true);
	//gameBoss.draw();

  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  gameBoss.setVisibility(true);
  draw(1);
  clock.pause();

  while (!done) {
    // The next loop polls for events, guarding against key bounce:
    while (SDL_PollEvent(&event)) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          return true;
        }
        if (keystate[SDL_SCANCODE_RETURN]) {
          if (gameBoss.isVisible()) {
            gameBoss.setVisibility(false);
            hud.setVisibility(true);
	          clock.unpause();
          }
	}
        if ( keystate[SDL_SCANCODE_P] || keystate[SDL_SCANCODE_O] ) {
          clock.pause();
          //menuEngine.play();
          clock.unpause();
        }
        if (keystate[SDL_SCANCODE_M]) {
          sound.toggleMusic();
	}
        if (keystate[SDL_SCANCODE_T]) {
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
        if (keystate[SDL_SCANCODE_SPACE]) {
          player->shoot();
	}
        if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
	if (keystate[SDL_SCANCODE_R]) {
          //clock.unpause();
          gameBoss.setVisibility(false);
          return false;
        }
        if (keystate[SDL_SCANCODE_G]) {
          player->toggleGodMode();
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
      if(clock.getTicks()%500 == 1){}
        //balloon->shoot();
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
      //balloon->draw();
      draw(1);
    //  balloon->update(ticks);
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();

    	}


}
  return true;
}
