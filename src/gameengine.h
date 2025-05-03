#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL.h"
#include <string>
#include "controls.h"
#include "font.h"

const int kScreenWidth = 256;
const int kScreenHeight = 240;

class GameEngine {
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
  Font* m_font;
  SDL_Event m_externalEvent;
#ifdef PSP
  SDL_Joystick* m_pJoystick;
#endif
  SDL_Rect m_camera;
  SDL_Rect m_cameraBoundary;
  SDL_Surface* m_pScreen;

  public:
  GameEngine();
  ~GameEngine();
  bool areColliding(SDL_Rect* a_pBoxA, SDL_Rect* a_pBoxB);
  void blitToScreen(int a_x, int a_y, SDL_Surface* a_pSourceBitmap, SDL_Rect* a_pSourceBitmapArea = 0);
  void blitToSurface(int a_x, int a_y, SDL_Surface* a_pSourceBitmap, SDL_Surface* a_destination, SDL_Rect* a_pSourceBitmapArea = 0);
  void blitUiToScreen(int a_x, int a_y, SDL_Surface* a_pSourceBitmap, SDL_Rect* a_pSourceBitmapArea = 0);
  void clearScreen();
  SDL_Surface* createSurface(int a_width, int a_height);
  int flipScreen();
  inline SDL_Rect getCamera() {
    return m_camera;
  }
  inline Controls getControls() {
    return m_buttons;
  }
  inline Font& getFont() {
    return *m_font;
  }
  void handleEvents();
  bool isOnCamera(SDL_Rect* a_pBox);
  SDL_Surface* loadSurface(const char* a_pFilename);
  void moveCamera(int a_xOffset, int a_yOffset);
  inline bool paused() {
    return m_paused;
  }
  inline bool quitRequested() {
    return m_quitRequest;
  }
  void setCameraBoundary(SDL_Rect a_boundary);
  void unloadSurface(SDL_Surface* a_pSurface);
  void updateCamera(SDL_Rect* a_pBox);
};

#endif
