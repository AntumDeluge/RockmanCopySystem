#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "gamestate.h"

class Enemy;
class GameStateManager;
class Map;
class Rockman;

class Level : public GameState {
  private:
  static const int kReadyCountdown = 180;
  static const int kReadyMessagePosY = 98;
  int m_readyCountdown;
  Enemy* m_pEnemy;
  GameStateManager* m_pGameStateManager;
  Map* m_pMap;
  Rockman* m_pRockman;
  SDL_Surface* m_pReadyBitmap;

  public:
  Level(GameStateManager* a_pGameStateManager);
  ~Level();
  void draw();
  void update();
};

#endif
