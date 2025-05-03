#include "gameengine.h"
#include "inidictionary.h"
#include "rect.h"
#include <cstring>


GameEngine::GameEngine()
  : m_paused(false)
  , m_quitRequest(false)
  , m_unpaused1Frame(false)
  , m_pixelMultiplier(1)
#ifdef PSP
  , m_pJoystick(0)
#endif
#ifndef SDL2
  , m_pScreen(0)
#else
  , m_pRenderer(0)
#endif
{
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
#if defined(WIN32) && !defined(SDL2)
  SDL_putenv("SDL_VIDEODRIVER=directx");
#endif
#ifndef PSP
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
#  ifndef SDL2
  SDL_WM_SetCaption("Rockman Copy System", 0);
#  endif
  IniDictionary settings("settings.ini");
  m_pixelMultiplier = settings.getIntValue("pixelMultiplier", "Video", 1);
  int multipliedScreenWidth = kScreenWidth * m_pixelMultiplier;
  int multipliedScreenHeight = kScreenHeight * m_pixelMultiplier;
#  ifndef SDL2
  m_pScreen = SDL_SetVideoMode(multipliedScreenWidth, multipliedScreenHeight, 0, SDL_SWSURFACE);
#  else
  SDL_Window* window = SDL_CreateWindow("Rockman Copy System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, multipliedScreenWidth, multipliedScreenHeight, SDL_WINDOW_SHOWN);
  m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
#  endif
#else
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_JoystickEventState(SDL_ENABLE);
  m_pJoystick = SDL_JoystickOpen(0);
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

void GameEngine::copyToBitmap(Bitmap& a_sourceBitmap, Bitmap& a_destinationBitmap, int a_destinationX, int a_destinationY, Rect* a_pSourceBitmapArea) {
  a_destinationX *= m_pixelMultiplier;
  a_destinationY *= m_pixelMultiplier;
  SDL_Rect sourceRect = {0, 0, a_sourceBitmap.width, a_sourceBitmap.height};
  if (a_pSourceBitmapArea != 0) {
#ifndef PSP
    sourceRect.x = a_pSourceBitmapArea->x * m_pixelMultiplier;
    sourceRect.y = a_pSourceBitmapArea->y * m_pixelMultiplier;
    sourceRect.w = a_pSourceBitmapArea->w * m_pixelMultiplier;
    sourceRect.h = a_pSourceBitmapArea->h * m_pixelMultiplier;
#else
    sourceRect.x = a_pSourceBitmapArea->x;
    sourceRect.y = a_pSourceBitmapArea->y;
    sourceRect.w = a_pSourceBitmapArea->w;
    sourceRect.h = a_pSourceBitmapArea->h;
#endif
  }
  SDL_Rect destinationRect = {a_destinationX, a_destinationY, sourceRect.w, sourceRect.h};
#ifndef SDL2
  SDL_BlitSurface(a_sourceBitmap.pInternalObject, &sourceRect, a_destinationBitmap.pInternalObject, &destinationRect);
#else
  SDL_SetRenderTarget(m_pRenderer, a_destinationBitmap.pInternalObject);
  SDL_RenderCopy(m_pRenderer, a_sourceBitmap.pInternalObject, &sourceRect, &destinationRect);
  SDL_SetRenderTarget(m_pRenderer, 0);
#endif
}

void GameEngine::copyToScreen(Bitmap& a_sourceBitmap, int a_x, int a_y, Rect* a_pSourceBitmapArea) {
  a_x *= m_pixelMultiplier;
  a_y *= m_pixelMultiplier;
  SDL_Rect sourceRect = {0, 0, a_sourceBitmap.width, a_sourceBitmap.height};
  if (a_pSourceBitmapArea != 0) {
#ifndef PSP
    sourceRect.x = a_pSourceBitmapArea->x * m_pixelMultiplier;
    sourceRect.y = a_pSourceBitmapArea->y * m_pixelMultiplier;
    sourceRect.w = a_pSourceBitmapArea->w * m_pixelMultiplier;
    sourceRect.h = a_pSourceBitmapArea->h * m_pixelMultiplier;
#else
    sourceRect.x = a_pSourceBitmapArea->x;
    sourceRect.y = a_pSourceBitmapArea->y;
    sourceRect.w = a_pSourceBitmapArea->w;
    sourceRect.h = a_pSourceBitmapArea->h;
#endif
  }
  SDL_Rect destinationRect = {a_x, a_y, sourceRect.w, sourceRect.h};
#ifndef SDL2
  SDL_BlitSurface(a_sourceBitmap.pInternalObject, &sourceRect, m_pScreen, &destinationRect);
#else
  SDL_RenderCopy(m_pRenderer, a_sourceBitmap.pInternalObject, &sourceRect, &destinationRect);
#endif
}

void GameEngine::clearScreen() {
#ifndef SDL2
  Uint32 black = SDL_MapRGB(m_pScreen->format, 0, 0, 0);
  SDL_FillRect(m_pScreen, 0, black);
#else
  SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
  SDL_RenderClear(m_pRenderer);
#endif
}

void GameEngine::createBitmap(Bitmap& a_bitmap, int a_width, int a_height) {
#ifndef PSP
  a_width *= m_pixelMultiplier;
  a_height *= m_pixelMultiplier;
#endif
#ifndef SDL2
  SDL_PixelFormat* pPixelFormat = m_pScreen->format;
  SDL_Surface* pNewSurface = SDL_CreateRGBSurface(0, a_width, a_height, pPixelFormat->BitsPerPixel, pPixelFormat->Rmask, pPixelFormat->Gmask, pPixelFormat->Bmask, pPixelFormat->Amask);
  Uint32 colorKey = SDL_MapRGB(pNewSurface->format, 0xFF, 0x80, 0xFF);
  SDL_FillRect(pNewSurface, 0, colorKey);
  SDL_SetColorKey(pNewSurface, SDL_SRCCOLORKEY, colorKey);
  a_bitmap.pInternalObject = pNewSurface;
#else
  SDL_Texture* pNewTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, a_width, a_height);
  SDL_SetTextureBlendMode(pNewTexture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(m_pRenderer, pNewTexture);
  SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0);
  SDL_RenderClear(m_pRenderer);
  SDL_SetRenderTarget(m_pRenderer, 0);
  a_bitmap.pInternalObject = pNewTexture;
#endif
  a_bitmap.width = a_width;
  a_bitmap.height = a_height;
}

int GameEngine::flipScreen() {
#ifndef SDL2
  return SDL_Flip(m_pScreen);
#else
  SDL_RenderPresent(m_pRenderer);
  return 0;
#endif
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
#ifndef PSP
    if (m_externalEvent.type == SDL_KEYDOWN) {
      switch (m_externalEvent.key.keysym.sym) {
#else
    if (m_externalEvent.type == SDL_JOYBUTTONDOWN) {
      switch (m_externalEvent.jbutton.button) {
#endif
#ifndef PSP
        case SDLK_UP:
#else
        case PspButton::Up:
#endif
          m_buttons.verticalDirection = Direction::Up;
          m_buttons.upDown = true;
          break;
#ifndef PSP
        case SDLK_DOWN:
#else
        case PspButton::Down:
#endif
          m_buttons.verticalDirection = Direction::Down;
          m_buttons.downDown = true;
          break;
#ifndef PSP
        case SDLK_LEFT:
#else
        case PspButton::Left:
#endif
          m_buttons.horizontalDirection = Direction::Left;
          m_buttons.leftDown = true;
          break;
#ifndef PSP
        case SDLK_RIGHT:
#else
        case PspButton::Right:
#endif
          m_buttons.horizontalDirection = Direction::Right;
          m_buttons.rightDown = true;
          break;
#ifndef PSP
        case SDLK_b:
#else
        case PspButton::Square:
#endif
          m_buttons.actionPressed = true;
          m_buttons.actionDown = true;
          m_buttons.actionReleased = false;
          break;
#ifndef PSP
        case SDLK_SPACE:
#else
        case PspButton::Cross:
#endif
          m_buttons.jumpPressed = true;
          m_buttons.jumpDown = true;
          m_buttons.jumpReleased = false;
          break;
#ifndef PSP
        case SDLK_RETURN:
#else
        case PspButton::Start:
#endif
          m_paused = !m_paused;
          break;
#ifndef PSP
        case SDLK_TAB:
#else
        case PspButton::Select:
#endif
          if (m_paused) {
            m_paused = false;
            m_unpaused1Frame = true;
          }
          break;
      }
    }
#ifndef PSP
    else if (m_externalEvent.type == SDL_KEYUP) {
      switch (m_externalEvent.key.keysym.sym) {
#else
    else if (m_externalEvent.type == SDL_JOYBUTTONUP) {
      switch (m_externalEvent.jbutton.button) {
#endif
#ifndef PSP
        case SDLK_UP:
#else
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
#ifndef PSP
        case SDLK_DOWN:
#else
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
#ifndef PSP
        case SDLK_LEFT:
#else
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
#ifndef PSP
        case SDLK_RIGHT:
#else
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
#ifndef PSP
        case SDLK_b:
#else
        case PspButton::Square:
#endif
          m_buttons.actionPressed = false;
          m_buttons.actionDown = false;
          m_buttons.actionReleased = true;
          break;
#ifndef PSP
        case SDLK_SPACE:
#else
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

void GameEngine::loadBitmap(Bitmap& a_bitmap, const char* a_pFilename) {
  SDL_Surface* pLoadedSurface = SDL_LoadBMP(a_pFilename);
  Uint32 colorKey = SDL_MapRGB(pLoadedSurface->format, 0xFF, 0x80, 0xFF);
#ifndef SDL2
  SDL_SetColorKey(pLoadedSurface, SDL_SRCCOLORKEY, colorKey);
#else
  SDL_SetColorKey(pLoadedSurface, SDL_TRUE, colorKey);
#endif
#ifndef SDL2
#  ifndef PSP
  SDL_Surface* pOptimisedSurface = SDL_DisplayFormat(pLoadedSurface);
#  else
  SDL_Surface* pOptimisedSurface = SDL_DisplayFormatAlpha(pLoadedSurface);
#  endif
  SDL_FreeSurface(pLoadedSurface);
  pLoadedSurface = pOptimisedSurface;
#endif
#ifndef PSP
  if (m_pixelMultiplier > 1) {
    Uint8* sourceBytes = (Uint8*)pLoadedSurface->pixels;
    int multipliedWidth = pLoadedSurface->w * m_pixelMultiplier;
    int multipliedHeight = pLoadedSurface->h * m_pixelMultiplier;
    SDL_PixelFormat* pSourcePixelFormat = pLoadedSurface->format;
    SDL_Surface* pMultipliedSurface = SDL_CreateRGBSurface(0, multipliedWidth, multipliedHeight, pSourcePixelFormat->BitsPerPixel, pSourcePixelFormat->Rmask, pSourcePixelFormat->Gmask, pSourcePixelFormat->Bmask, pSourcePixelFormat->Amask);
#  ifndef SDL2
    SDL_SetColorKey(pMultipliedSurface, SDL_SRCCOLORKEY, colorKey);
#  else
    SDL_SetColorKey(pMultipliedSurface, SDL_TRUE, colorKey);
#  endif
    Uint8* destinationBytes = (Uint8*)pMultipliedSurface->pixels;
    int multipliedPixelRowSize = pMultipliedSurface->w * pMultipliedSurface->format->BytesPerPixel;
    Uint8* multipliedPixelRow = new Uint8[multipliedPixelRowSize];
    for (int iSourceRowPixel = 0; iSourceRowPixel < pLoadedSurface->h; ++iSourceRowPixel) {
      int iSourceRowByte = iSourceRowPixel * pLoadedSurface->pitch;
      int iSourceByte = iSourceRowByte;
      for (int iSourceColumnPixel = 0; iSourceColumnPixel < pLoadedSurface->w; ++iSourceColumnPixel) {
        for (int iPixelMultiplier = 0; iPixelMultiplier < m_pixelMultiplier; ++iPixelMultiplier) {
          int iMultipliedPixelRow = ((iSourceColumnPixel * m_pixelMultiplier) + iPixelMultiplier) * pLoadedSurface->format->BytesPerPixel;
          memcpy(&(multipliedPixelRow[iMultipliedPixelRow]), &(sourceBytes[iSourceByte]), pLoadedSurface->format->BytesPerPixel);
        }
        iSourceByte += pLoadedSurface->format->BytesPerPixel;
      }
      int iDestinationByte = iSourceRowPixel * m_pixelMultiplier * pMultipliedSurface->pitch;
      for (int iPixelMultiplier = 0; iPixelMultiplier < m_pixelMultiplier; ++iPixelMultiplier) {
        memcpy(&(destinationBytes[iDestinationByte]), multipliedPixelRow, multipliedPixelRowSize);
        iDestinationByte += pMultipliedSurface->pitch;
      }
    }
    SDL_FreeSurface(pLoadedSurface);
    pLoadedSurface = pMultipliedSurface;
  }
#endif
#ifndef SDL2
  a_bitmap.pInternalObject = pLoadedSurface;
#else
  SDL_Texture* pLoadedTexture = SDL_CreateTextureFromSurface(m_pRenderer, pLoadedSurface);
  a_bitmap.pInternalObject = pLoadedTexture;
#endif
  a_bitmap.width = pLoadedSurface->w;
  a_bitmap.height = pLoadedSurface->h;
#ifdef SDL2
  SDL_FreeSurface(pLoadedSurface);
#endif
}

void GameEngine::unloadBitmap(Bitmap& a_bitmap) {
#ifndef SDL2
  SDL_FreeSurface(a_bitmap.pInternalObject);
#else
  SDL_DestroyTexture(a_bitmap.pInternalObject);
#endif
}
