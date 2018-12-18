// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

//ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
  srand(time(NULL));
  bool keepPlaying = true;
   try {
     while (keepPlaying) {
       Engine* engine = new Engine;
       keepPlaying = engine->play();
       delete engine;
       Clock::getInstance().startClock();
       SDL_Sound::getInstance().startMusic();
       Hud::getInstance().setVisibility(false);
       GameStart::getInstance().setVisibility(true);
       HealthBar::getInstance().reset();
}
      delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
