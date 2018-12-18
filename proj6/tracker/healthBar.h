#include "SDL.h"
#include "gameData.h"

class HealthBar {
public:
  static HealthBar& getInstance();

  void draw(int, int, int, int) const;
  void reset() { currentLength = 1.0; }
  void setCurrentLength(float l) { currentLength = l; }
  float getCurrentLength() const { return currentLength; }

private:
  SDL_Renderer* renderer;
  int backBarWidth;
  int backBarHeight;
  SDL_Color barColor;
  SDL_Color backColor;
  float currentLength;

  HealthBar();
  HealthBar(const HealthBar&);
  HealthBar& operator=(const HealthBar&) = delete;
};
