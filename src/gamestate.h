#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
  public:
  virtual ~GameState() {};
  virtual void draw() = 0;
  virtual void update() = 0;
};

#endif
