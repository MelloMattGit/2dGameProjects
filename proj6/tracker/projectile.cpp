#include <cmath>
#include "projectile.h"

void Projectile::update(Uint32 ticks) {
  setTimeSinceLastFrame(getTimeSinceLastFrame() + ticks);
  if (getTimeSinceLastFrame() > getFrameInterval()) {
    if (isTwoWay()) {
      if (getVelocityX() >= 0) {
        setCurrentFrame(((getCurrentFrame()+1) % (getNumFrames()/2)));
      } else {
        setCurrentFrame((getNumFrames()/2) + ((getCurrentFrame()+1) % (getNumFrames()/2)));
      }
    } else {
      setCurrentFrame((getCurrentFrame()+1) % getNumFrames());
    }
    setTimeSinceLastFrame(0);
  }

  setPosition(getPosition() + (getVelocity() * static_cast<float>(ticks) * 0.001));
  distance = (hypot(getPositionX()-startingPos[0], getPositionY()-startingPos[1]));
  if (distance > maxDistance) tooFar = true;
}
