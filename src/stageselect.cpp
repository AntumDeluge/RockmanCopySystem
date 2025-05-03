#include "stageselect.h"
#include "gamestatemanager.h"
#include "inidictionary.h"
#include "menuitem.h"
#include <string>


StageSelect::StageSelect(GameStateManager* a_pGameStateManager)
  : m_iSelectedLevelMenuItem(0)
  , m_iSelectedRockmanMenuItem(0)
  , m_nLevels(0)
  , m_nRockmans(0)
  , m_levelChosen(false)
  , m_pGameStateManager(a_pGameStateManager)
  , m_pLevelMenu(0)
  , m_pRockmanMenu(0)
  , m_pSelectorIconBitmap(0)
{
  IniDictionary resourcesDictionary("stageselect.ini");
  m_nLevels = resourcesDictionary.getIntValue("amount", "Levels", 0);
  m_pLevelMenu = new MenuItem[m_nLevels];
  int x = kLevelMenuPosX;
  int y = kLevelMenuPosY;
  Font& font = gameEngine->getFont();
  for (int iLevel = 0; iLevel < m_nLevels; iLevel++) {
    char indexString[2];
    sprintf(indexString, "%d", iLevel);
    std::string key("level");
    key += indexString;
    const char* pValue = resourcesDictionary.getCStringValue(key.c_str(), "Levels", "empty");
    MenuItem menuItem(pValue, pValue, x, y);
    m_pLevelMenu[iLevel] = menuItem;
    y += font.getCharacterHeight();
  }
  m_nRockmans = resourcesDictionary.getIntValue("amount", "Rockmans", 0);
  m_pRockmanMenu = new MenuItem[m_nRockmans];
  x = kRockmanMenuPosX;
  y = kRockmanMenuPosY;
  for (int iRockman = 0; iRockman < m_nRockmans; iRockman++) {
    char indexString[2];
    sprintf(indexString, "%d", iRockman);
    std::string key("rockman");
    key += indexString;
    const char* pValue = resourcesDictionary.getCStringValue(key.c_str(), "Rockmans", "empty");
    key += "title";
    const char* pCaption = resourcesDictionary.getCStringValue(key.c_str(), "Rockmans", "empty");
    MenuItem menuItem(pCaption, pValue, x, y);
    m_pRockmanMenu[iRockman] = menuItem;
    y += font.getCharacterHeight();
  }
  m_pSelectorIconBitmap = gameEngine->loadSurface("selector.bmp");
}

StageSelect::~StageSelect() {
  gameEngine->unloadSurface(m_pSelectorIconBitmap);
  delete[] m_pRockmanMenu;
  delete[] m_pLevelMenu;
}

void StageSelect::draw() {
  gameEngine->clearScreen();
  for (int iLevel = 0; iLevel < m_nLevels; iLevel++) {
    m_pLevelMenu[iLevel].draw();
  }
  for (int iRockman = 0; iRockman < m_nRockmans; iRockman++) {
    m_pRockmanMenu[iRockman].draw();
  }
  int selectorIconX = m_pLevelMenu[m_iSelectedLevelMenuItem].getX();
  int selectorIconY = m_pLevelMenu[m_iSelectedLevelMenuItem].getY();
  // Place the icon in front of the menu item.
  selectorIconX -= m_pSelectorIconBitmap->w;
  gameEngine->blitUiToScreen(selectorIconX, selectorIconY, m_pSelectorIconBitmap);
  if (m_levelChosen) {
    selectorIconX = m_pRockmanMenu[m_iSelectedRockmanMenuItem].getX();
    selectorIconY = m_pRockmanMenu[m_iSelectedRockmanMenuItem].getY();
    // Place the icon in front of the menu item.
    selectorIconX -= m_pSelectorIconBitmap->w;
    gameEngine->blitUiToScreen(selectorIconX, selectorIconY, m_pSelectorIconBitmap);
  }
}

void StageSelect::update() {
  Controls buttons = gameEngine->getControls();
  if (buttons.verticalDirection != buttons.previousVerticalDirection) {
    if (buttons.verticalDirection == Direction::Down) {
      if (!m_levelChosen) {
        int iLastLevelMenuItem = m_nLevels - 1;
        if (m_iSelectedLevelMenuItem < iLastLevelMenuItem) {
          m_iSelectedLevelMenuItem++;
        }
      }
      else {
        int iLastRockmanMenuItem = m_nRockmans - 1;
        if (m_iSelectedRockmanMenuItem < iLastRockmanMenuItem) {
          m_iSelectedRockmanMenuItem++;
        }
      }
    }
    else if (buttons.verticalDirection == Direction::Up) {
      if (!m_levelChosen) {
        if (m_iSelectedLevelMenuItem > 0) {
          m_iSelectedLevelMenuItem--;
        }
      }
      else {
        if (m_iSelectedRockmanMenuItem > 0) {
          m_iSelectedRockmanMenuItem--;
        }
      }
    }
  }
  if (buttons.actionPressed) {
    if (!m_levelChosen) {
      m_levelChosen = true;
    }
    else {
      std::string chosenLevel = m_pLevelMenu[m_iSelectedLevelMenuItem].getValue();
      std::string chosenRockman = m_pRockmanMenu[m_iSelectedRockmanMenuItem].getValue();
      m_pGameStateManager->setInitMessage(GameStateManager::InitMessageId::Level, chosenLevel.c_str());
      m_pGameStateManager->setInitMessage(GameStateManager::InitMessageId::Rockman, chosenRockman.c_str());
      m_pGameStateManager->setNextState();
    }
  }
  if (buttons.jumpPressed && m_levelChosen) {
    m_levelChosen = false;
  }
}
