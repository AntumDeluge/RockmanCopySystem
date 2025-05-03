#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "SDL.h"
#include <string>
#include "controls.h"
#include "font.h"
#include "videoservice.h"

struct Rect;

class GameEngine : public VideoService {
  private:
  static const int kScreenWidth = 256;
  static const int kScreenHeight = 240;
#  ifdef PSP
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
#  endif
  bool m_paused;
  bool m_quitRequest;
  bool m_unpaused1Frame;
  int m_pixelMultiplier;
  Controls m_buttons;
  SDL_Event m_externalEvent;
#  ifdef PSP
  SDL_Joystick* m_pJoystick;
#  endif
#  ifndef SDL2
  SDL_Surface* m_pScreen;
#  else
  SDL_Renderer* m_pRenderer;
#  endif

  public:
  GameEngine();
  ~GameEngine();
  bool areColliding(SDL_Rect* a_pBoxA, SDL_Rect* a_pBoxB);
  void copyToBitmap(Bitmap& a_sourceBitmap, Bitmap& a_destinationBitmap, int a_destinationX, int a_destinationY, Rect* a_pSourceBitmapArea = 0);
  void copyToScreen(Bitmap& a_sourceBitmap, int a_x, int a_y, Rect* a_pSourceBitmapArea = 0);
  void clearScreen();
  void createBitmap(Bitmap& a_bitmap, int a_width, int a_height);
  int flipScreen();
  inline Controls getControls() {
    return m_buttons;
  }
  void handleEvents();
  void loadBitmap(Bitmap& a_bitmap, const char* a_pFilename);
  inline bool paused() {
    return m_paused;
  }
  inline bool quitRequested() {
    return m_quitRequest;
  }
  void unloadBitmap(Bitmap& a_bitmap);
};

#endif
