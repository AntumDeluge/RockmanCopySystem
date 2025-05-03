#include "level.h"
#include "enemy.h"
#include "gamestatemanager.h"
#include "inidictionary.h"
#include "map.h"
#include "rockman.h"
#include <string>


Level::Level(GameStateManager* a_pGameStateManager)
  : m_readyCountdown(kReadyCountdown)
  , m_pEnemy(0)
  , m_pGameStateManager(a_pGameStateManager)
  , m_pMap(0)
  , m_pRockman(0)
  , m_pReadyBitmap(0)
{
  std::string levelName = m_pGameStateManager->getInitMessage(GameStateManager::InitMessageId::Level);
  std::string rockmanType = m_pGameStateManager->getInitMessage(GameStateManager::InitMessageId::Rockman);
  IniDictionary iniDictionary("levels.ini");
  std::string readyMessage(iniDictionary.getCStringValue("readyMessage", "Common", "empty"));
  Font& font = gameEngine->getFont();
  int readyMessageWidth = readyMessage.length() * font.getCharacterWidth();
  int readyMessageHeight = font.getCharacterHeight();
  m_pReadyBitmap = gameEngine->createSurface(readyMessageWidth, readyMessageHeight);
  font.drawText(readyMessage, m_pReadyBitmap);
  m_pMap = new Map(levelName.c_str(), iniDictionary);
  m_pRockman = new Rockman(rockmanType.c_str(), m_pMap);
  m_pEnemy = new Enemy();
}

Level::~Level() {
  gameEngine->unloadSurface(m_pReadyBitmap);
  delete m_pEnemy;
  delete m_pRockman;
  delete m_pMap;
}

void Level::draw() {
  if (m_readyCountdown == 0) {
    m_pMap->draw();
    m_pEnemy->draw();
    m_pRockman->draw();
  }
  else {
    m_pMap->draw();
    // Center the message.
    int readyMessagePosX = (kScreenWidth / 2) - (m_pReadyBitmap->w / 2);
    gameEngine->blitUiToScreen(readyMessagePosX, kReadyMessagePosY, m_pReadyBitmap, 0);
    m_readyCountdown--;
  }
}

void Level::update() {
  if (m_pRockman->isDestroyed()) {
    m_pRockman->reset();
    m_readyCountdown = kReadyCountdown;
  }
  if (m_readyCountdown == 0) {
    m_pRockman->update();
    m_pEnemy->update();
    Collider rockmanCollider = m_pRockman->getCollider();
    Collider enemyCollider = m_pEnemy->getCollider();
    bool collides = rockmanCollider.collidesWith(&enemyCollider);
    if (collides) {
      m_pEnemy->hit(m_pRockman);
    }
  }
}
