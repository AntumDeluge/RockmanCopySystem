#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL.h"
#include <string>
#include "controls.h"
#include "font.h"
#include "videoservice.h"

const int kScreenWidth = 256;
const int kScreenHeight = 240;

class GameEngine : public VideoService {
  private:
#ifdef PSP
  struct PspButton {
    enum type {
      Triangle = 0,
      Circle = 1,
      Cross = 2,
      Square = 3,
      L = 4,
      R = 5,
      Down = 6,
      Left = 7,
      Up = 8,
      Right = 9,
      Select = 10,
      Start = 11,
      Home = 12,
      Hold = 13
    };
  };
#endif
  bool m_paused;
  bool m_quitRequest;
  bool m_unpaused1Frame;
  Controls m_buttons;
  SDL_Event m_externalEvent;
#ifdef PSP
  SDL_Joystick* m_pJoystick;
#endif
  SDL_Surface* m_pScreen;

  public:
  GameEngine();
  ~GameEngine();
  bool areColliding(SDL_Rect* a_pBoxA, SDL_Rect* a_pBoxB);
  void blitToScreen(SDL_Surface* a_pSourceBitmap, int a_x, int a_y, SDL_Rect* a_pSourceBitmapArea = 0);
  void blitToSurface(SDL_Surface* a_pSourceBitmap, SDL_Surface* a_pDestinationBitmap, int a_destinationX, int a_destinationY, SDL_Rect* a_pSourceBitmapArea = 0);
  void blitUiToScreen(SDL_Surface* a_pSourceBitmap, int a_destinationX, int a_destinationY, SDL_Rect* a_pSourceBitmapArea = 0);
  void clearScreen();
  SDL_Surface* createSurface(int a_width, int a_height);
  int flipScreen();
  inline Controls getControls() {
    return m_buttons;
  }
  void handleEvents();
  SDL_Surface* loadSurface(const char* a_pFilename);
  inline bool paused() {
    return m_paused;
  }
  inline bool quitRequested() {
    return m_quitRequest;
  }
  void unloadSurface(SDL_Surface* a_pSurface);
};

#endif
