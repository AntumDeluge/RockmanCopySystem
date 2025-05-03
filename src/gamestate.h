#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "controls.h"

class GameState {
  public:
  virtual ~GameState() {};
  virtual void draw() = 0;
  virtual void update(Controls a_controls) = 0;
};

#endif
