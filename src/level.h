#ifndef LEVEL_H
#define LEVEL_H

#include "bitmap.h"
#include "camera.h"
#include "controls.h"
#include "gamestate.h"

class Enemy;
class GameStateManager;
class Map;
class Rockman;

class Level : public GameState {
  private:
  static const int kReadyMessagePosY = 98;
  static const int kScreenHeight = 240;
  static const int kScreenWidth = 256;
  static const int kScrollYSpeed = 4;
  struct State {
    enum type {
      Waiting = 0,
      Playing = 1,
      BeforeScrolling = 2,
      Scrolling = 3,
      AfterScrolling = 4
    };
  };
  bool m_stateChanged;
  bool m_displayReadyMessage;
  int m_duration;
  int m_scrollDuration;
  int m_beforeScrollDuration;
  int m_afterScrollDuration;
  int m_verticalScrollSpeed;
  int m_readyDuration;
  int m_readyMessageDisplayInterval;
  int m_readyMessageWidth;
  Direction::type m_scrollDirection;
  State::type m_state;
  Bitmap m_readyBitmap;
  Camera m_camera;
  Enemy* m_pEnemy;
  GameStateManager* m_pGameStateManager;
  Map* m_pMap;
  Rockman* m_pRockman;

  public:
  Level(GameStateManager* a_pGameStateManager);
  ~Level();
  void draw();
  void onReset();
  void onScrollStart(Direction::type a_direction);
  void update(Controls a_controls);
};

#endif
