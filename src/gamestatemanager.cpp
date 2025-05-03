#include "gamestatemanager.h"
#include "level.h"
#include "stageselect.h"


GameStateManager::GameStateManager()
  : m_nextState(false)
  , m_pCurrentState(0) {
  m_initMessages[InitMessageId::Level] = "Cutman";
  m_initMessages[InitMessageId::Rockman] = "rockman1";
  m_pCurrentState = new StageSelect(this);
}

GameStateManager::~GameStateManager() {
  delete m_pCurrentState;
}

void GameStateManager::draw() {
  m_pCurrentState->draw();
}

std::string GameStateManager::getInitMessage(InitMessageId::type a_id) {
  return m_initMessages[a_id];
}

void GameStateManager::setInitMessage(InitMessageId::type a_id, const char* a_pMessage) {
  m_initMessages[a_id] = a_pMessage;
}

void GameStateManager::setNextState() {
  m_nextState = true;
}

void GameStateManager::update(Controls a_controls) {
  if (!m_nextState) {
    m_pCurrentState->update(a_controls);
  }
  else {
    delete m_pCurrentState;
    m_pCurrentState = new Level(this);
    m_nextState = false;
  }
}
