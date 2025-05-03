#ifndef STAGESELECT_H
#define STAGESELECT_H

#include "bitmap.h"
#include "controls.h"
#include "font.h"
#include "gamestate.h"

class GameStateManager;
class MenuItem;

class StageSelect : public GameState {
  private:
  static const int kLevelMenuPosX = 32;
  static const int kLevelMenuPosY = 32;
  static const int kRockmanMenuPosX = 128;
  static const int kRockmanMenuPosY = 32;
  int m_iSelectedLevelMenuItem;
  int m_iSelectedRockmanMenuItem;
  int m_nLevels;
  int m_nRockmans;
  bool m_levelChosen;
  Bitmap m_selectorIconBitmap;
  GameStateManager* m_pGameStateManager;
  Font m_font;
  MenuItem* m_pLevelMenu;
  MenuItem* m_pRockmanMenu;

  public:
  StageSelect(GameStateManager* a_pGameStateManager);
  ~StageSelect();
  void draw();
  void update(Controls a_controls);
};

#endif
