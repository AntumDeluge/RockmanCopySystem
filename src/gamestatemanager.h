#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <string>

class GameState;

class GameStateManager {
  private:
  static const int knInitMessages = 2;
  bool m_nextState;
  std::string m_initMessages[knInitMessages];
  GameState* m_pCurrentState;

  public:
  struct InitMessageId {
    enum type {
      Level = 0,
      Rockman = 1
    };
  };
  GameStateManager();
  ~GameStateManager();
  void draw();
  std::string getInitMessage(InitMessageId::type a_id);
  void setInitMessage(InitMessageId::type a_id, const char* a_pMessage);
  void setNextState();
  void update();
};

#endif
