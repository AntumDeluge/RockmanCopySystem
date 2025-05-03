#include "level.h"
#include "enemy.h"
#include "font.h"
#include "gamestatemanager.h"
#include "inidictionary.h"
#include "map.h"
#include "rockman.h"
#include "servicelocator.h"
#include <string>


Level::Level(GameStateManager* a_pGameStateManager)
  : m_stateChanged(false)
  , m_duration(0)
  , m_scrollDuration(0)
  , m_beforeScrollDuration(0)
  , m_afterScrollDuration(0)
  , m_verticalScrollSpeed(0)
  , m_scrollDirection(Direction::None)
  , m_state(State::Waiting)
  , m_pEnemy(0)
  , m_pGameStateManager(a_pGameStateManager)
  , m_pMap(0)
  , m_pRockman(0)
  , m_pReadyBitmap(0)
{
  std::string levelName = m_pGameStateManager->getInitMessage(GameStateManager::InitMessageId::Level);
  std::string rockmanType = m_pGameStateManager->getInitMessage(GameStateManager::InitMessageId::Rockman);
  IniDictionary levelConstants("levels.ini");
  std::string readyMessage(levelConstants.getCStringValue("readyMessage", "Common", "empty"));
  Font font("font.ini");
  int readyMessageWidth = readyMessage.length() * font.getCharacterWidth();
  int readyMessageHeight = font.getCharacterHeight();
  VideoService& videoService = ServiceLocator::getVideoService();
  m_pReadyBitmap = videoService.createSurface(readyMessageWidth, readyMessageHeight);
  font.drawText(readyMessage, m_pReadyBitmap);
  m_pMap = new Map(levelName.c_str(), levelConstants);
  IniDictionary rockmanConstants(rockmanType.c_str());
  m_scrollDuration = rockmanConstants.getIntValue("scrolling", "Durations", 0);
  m_beforeScrollDuration = rockmanConstants.getIntValue("beforeScrolling", "Durations", 0);
  m_afterScrollDuration = rockmanConstants.getIntValue("afterScrolling", "Durations", 0);
  m_verticalScrollSpeed = rockmanConstants.getIntValue("verticalSpeed", "ScrollConstants", 0);
  int scrollStartPosTop = rockmanConstants.getIntValue("startPosTop", "ScrollConstants", 0);
  int scrollStartPosBottom = rockmanConstants.getIntValue("startPosBottom", "ScrollConstants", kScreenHeight);
  m_pMap->setScrollStartPositions(scrollStartPosTop, scrollStartPosBottom);
  m_pRockman = new Rockman(rockmanConstants, this, m_pMap);
  const SDL_Rect& rockmanBoundingBox = m_pRockman->getBoundingBox();
  m_pMap->setCurrentBoundaryByPosition(rockmanBoundingBox.x, rockmanBoundingBox.y);
  SDL_Rect currentBoundary = m_pMap->getCurrentBoundary();
  m_camera.setCurrentBoundary(currentBoundary);
  m_camera.update(rockmanBoundingBox);
  m_pEnemy = new Enemy();
}

Level::~Level() {
  VideoService& videoService = ServiceLocator::getVideoService();
  videoService.unloadSurface(m_pReadyBitmap);
  delete m_pEnemy;
  delete m_pRockman;
  delete m_pMap;
}

void Level::draw() {
  if (m_state != State::Waiting) {
    m_pMap->draw(m_camera);
    m_pEnemy->draw(m_camera);
    m_pRockman->draw(m_camera);
  }
  else {
    m_pMap->draw(m_camera);
    // Center the message.
    int readyMessagePosX = (kScreenWidth / 2) - (m_pReadyBitmap->w / 2);
    VideoService& videoService = ServiceLocator::getVideoService();
    videoService.blitToScreen(m_pReadyBitmap, readyMessagePosX, kReadyMessagePosY, 0);
    if (m_duration == kWaitingDuration) {
      m_state = State::Playing;
      m_duration = 0;
    }
  }
}

void Level::onReset() {
  int rockmanPosX = m_pRockman->getX();
  int rockmanPosY = m_pRockman->getY();
  m_pMap->setCurrentBoundaryByPosition(rockmanPosX, rockmanPosY);
  SDL_Rect currentBoundary = m_pMap->getCurrentBoundary();
  m_camera.setCurrentBoundary(currentBoundary);
  m_state = State::Waiting;
  m_duration = 0;
  m_stateChanged = true;
}

void Level::onScrollStart(Direction::type a_direction) {
  m_scrollDirection = a_direction;
  if (m_beforeScrollDuration > 0) {
    m_state = State::BeforeScrolling;
  }
  else {
    m_state = State::Scrolling;
    if (m_scrollDirection == Direction::Up) {
      m_camera.move(0, -m_verticalScrollSpeed);
    }
    else if (m_scrollDirection == Direction::Down) {
      m_camera.move(0, m_verticalScrollSpeed);
    }
  }
  m_duration = 0;
  m_stateChanged = true;
}

void Level::update(Controls a_controls) {
  if (m_state == State::Playing) {
    m_pRockman->update(a_controls, m_camera);
    const SDL_Rect& rockmanBoundingBox = m_pRockman->getBoundingBox();
    m_camera.update(rockmanBoundingBox);
    if (!m_stateChanged) {
      m_pEnemy->update();
      Collider rockmanCollider = m_pRockman->getCollider();
      Collider enemyCollider = m_pEnemy->getCollider();
      bool collides = rockmanCollider.collidesWith(&enemyCollider);
      if (collides) {
        m_pEnemy->hit(m_pRockman);
      }
    }
  }
  else if (m_state == State::BeforeScrolling) {
    if (m_duration == m_beforeScrollDuration) {
      m_state = State::Scrolling;
      m_duration = 0;
      m_stateChanged = true;
    }
  }
  else if (m_state == State::Scrolling) {
    if (m_duration == m_scrollDuration) {
      int rockmanPosX = m_pRockman->getX();
      int rockmanPosY = m_pRockman->getY();
      m_pMap->setCurrentBoundaryByPosition(rockmanPosX, rockmanPosY);
      SDL_Rect currentBoundary = m_pMap->getCurrentBoundary();
      m_camera.setCurrentBoundary(currentBoundary);
      m_state = State::AfterScrolling;
      m_duration = 0;
      m_stateChanged = true;
    }
    else {
      m_pRockman->update(a_controls, m_camera);
      if (m_scrollDirection == Direction::Up) {
        m_camera.move(0, -m_verticalScrollSpeed);
      }
      else if (m_scrollDirection == Direction::Down) {
        m_camera.move(0, m_verticalScrollSpeed);
      }
    }
  }
  else if (m_state == State::AfterScrolling) {
    if (m_duration == m_afterScrollDuration) {
      m_pRockman->onScrollingStop();
      m_pRockman->update(a_controls, m_camera);
      m_state = State::Playing;
      m_duration = 0;
      m_stateChanged = true;
    }
  }
  m_duration++;
  m_stateChanged = false;
}
