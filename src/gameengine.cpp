#include "gameengine.h"


GameEngine::GameEngine()
  : m_paused(false)
  , m_quitRequest(false)
  , m_unpaused1Frame(false) {
#ifdef WIN32
  SDL_putenv("SDL_VIDEODRIVER=directx");
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
  SDL_WM_SetCaption("Rockman Copy System", 0);
#endif
#ifdef PSP
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_JoystickEventState(SDL_ENABLE);
  m_pJoystick = SDL_JoystickOpen(0);
#endif
  m_buttons.horizontalDirection = Direction::None;
  m_buttons.verticalDirection = Direction::None;
  m_buttons.previousHorizontalDirection = Direction::None;
  m_buttons.previousVerticalDirection = Direction::None;
  m_buttons.upDown = false;
  m_buttons.downDown = false;
  m_buttons.leftDown = false;
  m_buttons.rightDown = false;
  m_buttons.actionPressed = false;
  m_buttons.actionDown = false;
  m_buttons.actionReleased = false;
  m_buttons.jumpPressed = false;
  m_buttons.jumpDown = false;
  m_buttons.jumpReleased = false;
#ifdef WIN32
  m_pScreen = SDL_SetVideoMode(kScreenWidth, kScreenHeight, 0, SDL_SWSURFACE);
#endif
#ifdef PSP
  m_pScreen = SDL_SetVideoMode(kScreenWidth, kScreenHeight, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
#endif
}

GameEngine::~GameEngine() {
#ifdef PSP
  SDL_JoystickClose(m_pJoystick);
#endif
  SDL_Quit();
}

bool GameEngine::areColliding(SDL_Rect* a_pBoxA, SDL_Rect* a_pBoxB) {
  int leftBoxA = a_pBoxA->x;
  int rightBoxA = a_pBoxA->x + a_pBoxA->w;
  int topBoxA = a_pBoxA->y;
  int bottomBoxA = a_pBoxA->y + a_pBoxA->h;
  int leftBoxB = a_pBoxB->x;
  int rightBoxB = a_pBoxB->x + a_pBoxB->w;
  int topBoxB = a_pBoxB->y;
  int bottomBoxB = a_pBoxB->y + a_pBoxB->h;
  if (bottomBoxA <= topBoxB) {
    return false;
  }
  if (topBoxA >= bottomBoxB) {
    return false;
  }
  if (rightBoxA <= leftBoxB) {
    return false;
  }
  if (leftBoxA >= rightBoxB) {
    return false;
  }
  return true;
}

void GameEngine::blitToScreen(SDL_Surface* a_pSourceBitmap, int a_x, int a_y, SDL_Rect* a_pSourceBitmapArea) {
  SDL_Rect destinationRect = {a_x, a_y, 0, 0};
  SDL_BlitSurface(a_pSourceBitmap, a_pSourceBitmapArea, m_pScreen, &destinationRect);
}

void GameEngine::blitToSurface(SDL_Surface* a_pSourceBitmap, SDL_Surface* a_pDestinationBitmap, int a_destinationX, int a_destinationY, SDL_Rect* a_pSourceBitmapArea) {
  SDL_Rect destinationRect = {a_destinationX, a_destinationY, 0, 0};
  SDL_BlitSurface(a_pSourceBitmap, a_pSourceBitmapArea, a_pDestinationBitmap, &destinationRect);
}

void GameEngine::blitUiToScreen(SDL_Surface* a_pSourceBitmap, int a_destinationX, int a_destinationY, SDL_Rect* a_pSourceBitmapArea) {
  SDL_Rect offset;
  offset.x = a_destinationX;
  offset.y = a_destinationY;
  SDL_BlitSurface(a_pSourceBitmap, a_pSourceBitmapArea, m_pScreen, &offset);
}

void GameEngine::clearScreen() {
  Uint32 black = SDL_MapRGB(m_pScreen->format, 0, 0, 0);
  SDL_FillRect(m_pScreen, 0, black);
}

SDL_Surface* GameEngine::createSurface(int a_width, int a_height) {
  SDL_PixelFormat* pPixelFormat = m_pScreen->format;
  SDL_Surface* pNewSurface = SDL_CreateRGBSurface(0, a_width, a_height, pPixelFormat->BitsPerPixel, pPixelFormat->Rmask, pPixelFormat->Gmask, pPixelFormat->Bmask, pPixelFormat->Amask);
  Uint32 colorKey = SDL_MapRGB(pNewSurface->format, 0xFF, 0x80, 0xFF);
  SDL_FillRect(pNewSurface, 0, colorKey);
  SDL_SetColorKey(pNewSurface, SDL_SRCCOLORKEY, colorKey);
  return pNewSurface;
}

int GameEngine::flipScreen() {
  return SDL_Flip(m_pScreen);
}

void GameEngine::handleEvents() {
  if (!m_paused) {
    m_buttons.previousHorizontalDirection = m_buttons.horizontalDirection;
    m_buttons.previousVerticalDirection = m_buttons.verticalDirection;
    m_buttons.actionPressed = false;
    m_buttons.actionReleased = false;
    m_buttons.jumpPressed = false;
    m_buttons.jumpReleased = false;
  }
  if (m_unpaused1Frame) {
    m_paused = true;
    m_unpaused1Frame = false;
  }
  while (SDL_PollEvent(&m_externalEvent)) {
#ifdef WIN32
    if (m_externalEvent.type == SDL_KEYDOWN) {
      switch (m_externalEvent.key.keysym.sym) {
#endif
#ifdef PSP
    if (m_externalEvent.type == SDL_JOYBUTTONDOWN) {
      switch (m_externalEvent.jbutton.button) {
#endif
#ifdef WIN32
        case SDLK_UP:
#endif
#ifdef PSP
        case PspButton::Up:
#endif
          m_buttons.verticalDirection = Direction::Up;
          m_buttons.upDown = true;
          break;
#ifdef WIN32
        case SDLK_DOWN:
#endif
#ifdef PSP
        case PspButton::Down:
#endif
          m_buttons.verticalDirection = Direction::Down;
          m_buttons.downDown = true;
          break;
#ifdef WIN32
        case SDLK_LEFT:
#endif
#ifdef PSP
        case PspButton::Left:
#endif
          m_buttons.horizontalDirection = Direction::Left;
          m_buttons.leftDown = true;
          break;
#ifdef WIN32
        case SDLK_RIGHT:
#endif
#ifdef PSP
        case PspButton::Right:
#endif
          m_buttons.horizontalDirection = Direction::Right;
          m_buttons.rightDown = true;
          break;
#ifdef WIN32
        case SDLK_b:
#endif
#ifdef PSP
        case PspButton::Square:
#endif
          m_buttons.actionPressed = true;
          m_buttons.actionDown = true;
          m_buttons.actionReleased = false;
          break;
#ifdef WIN32
        case SDLK_SPACE:
#endif
#ifdef PSP
        case PspButton::Cross:
#endif
          m_buttons.jumpPressed = true;
          m_buttons.jumpDown = true;
          m_buttons.jumpReleased = false;
          break;
#ifdef WIN32
        case SDLK_RETURN:
#endif
#ifdef PSP
        case PspButton::Start:
#endif
          m_paused = !m_paused;
          break;
#ifdef WIN32
        case SDLK_TAB:
#endif
#ifdef PSP
        case PspButton::Select:
#endif
          if (m_paused) {
            m_paused = false;
            m_unpaused1Frame = true;
          }
          break;
      }
    }
#ifdef WIN32
    else if (m_externalEvent.type == SDL_KEYUP) {
      switch (m_externalEvent.key.keysym.sym) {
#endif
#ifdef PSP
    else if (m_externalEvent.type == SDL_JOYBUTTONUP) {
      switch (m_externalEvent.jbutton.button) {
#endif
#ifdef WIN32
        case SDLK_UP:
#endif
#ifdef PSP
        case PspButton::Up:
#endif
          if (m_buttons.downDown) {
            m_buttons.verticalDirection = Direction::Down;
          }
          else {
            m_buttons.verticalDirection = Direction::None;
          }
          m_buttons.upDown = false;
          break;
#ifdef WIN32
        case SDLK_DOWN:
#endif
#ifdef PSP
        case PspButton::Down:
#endif
          if (m_buttons.upDown) {
            m_buttons.verticalDirection = Direction::Up;
          }
          else {
            m_buttons.verticalDirection = Direction::None;
          }
          m_buttons.downDown = false;
          break;
#ifdef WIN32
        case SDLK_LEFT:
#endif
#ifdef PSP
        case PspButton::Left:
#endif
          if (m_buttons.rightDown) {
            m_buttons.horizontalDirection = Direction::Right;
          }
          else {
            m_buttons.horizontalDirection = Direction::None;
          }
          m_buttons.leftDown = false;
          break;
#ifdef WIN32
        case SDLK_RIGHT:
#endif
#ifdef PSP
        case PspButton::Right:
#endif
          if (m_buttons.leftDown) {
            m_buttons.horizontalDirection = Direction::Left;
          }
          else {
            m_buttons.horizontalDirection = Direction::None;
          }
          m_buttons.rightDown = false;
          break;
#ifdef WIN32
        case SDLK_b:
#endif
#ifdef PSP
        case PspButton::Square:
#endif
          m_buttons.actionPressed = false;
          m_buttons.actionDown = false;
          m_buttons.actionReleased = true;
          break;
#ifdef WIN32
        case SDLK_SPACE:
#endif
#ifdef PSP
        case PspButton::Cross:
#endif
          m_buttons.jumpPressed = false;
          m_buttons.jumpDown = false;
          m_buttons.jumpReleased = true;
          break;
      }
    }
    else if (m_externalEvent.type == SDL_QUIT) {
      m_quitRequest = true;
    }
  }
}

SDL_Surface* GameEngine::loadSurface(const char* a_pFilename) {
  SDL_Surface* pLoadedSurface = SDL_LoadBMP(a_pFilename);
  Uint32 colorKey = SDL_MapRGB(pLoadedSurface->format, 0xFF, 0x80, 0xFF);
  SDL_SetColorKey(pLoadedSurface, SDL_SRCCOLORKEY, colorKey);
#ifdef PSP
  SDL_Surface* pOptimisedSurface = SDL_DisplayFormatAlpha(pLoadedSurface);
  SDL_FreeSurface(pLoadedSurface);
  pLoadedSurface = pOptimisedSurface;
#endif
  return pLoadedSurface;
}

void GameEngine::unloadSurface(SDL_Surface* a_pSurface) {
  SDL_FreeSurface(a_pSurface);
}
