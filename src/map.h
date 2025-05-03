#ifndef MAP_H
#define MAP_H

#include "globals.h"
#include "direction.h"
#include "fixedpoint8.h"
#include "inidictionary.h"
#include "laddersonar.h"
#include "tile.h"

class Map {
  private:
  static const int knTilesPerColumnInScreen = kScreenHeight / Tile::kWidth;
  static const int knTilesPerRowInScreen = kScreenWidth / Tile::kWidth;
  static const int kScrollStartPosTop = 4;
  static const int kScrollStartPosBottom = 232;
  static const int kTeleportStartPosXInt = kScreenWidth / 2;
  static const int kTeleportStartPosYInt = 4;
  static const int kTeleportStartPosYFraction = 248;
  static const int knTileBytes = 2;
  int m_width, m_height;
  int m_nBoundaries, m_currentBoundaryIndex;
  int m_nTiles, m_nTileTypes;
  int m_nScreens, m_nScreensHorizontal, m_nScreensVertical;
  int m_teleportStopPosY;
  FixedPoint8 m_teleportStartYSpeed;
  SDL_Rect* m_pBoundaries;
  SDL_Rect* m_pTileSetBitmapAreas;
  SDL_Surface* m_pTileSetBitmap;
  Tile** m_ppScreens;

  public:
  Map(const char* a_pMapName, IniDictionary& a_iniDictionary);
  ~Map();
  void draw();
  SDL_Rect getCurrentBoundary();
  inline int getHeight() {
    return m_height;
  }
  int getMaxXDelta(SDL_Rect a_boundingBox, Direction::type a_direction);
  int getMaxYDelta(SDL_Rect a_boundingBox, Direction::type a_direction);
  LadderSonar getLadderSonar(int a_x, int a_y, int a_yNextFrame, bool a_movingUp);
  FixedPoint8 getTeleportStartPosX();
  FixedPoint8 getTeleportStartPosY();
  inline FixedPoint8 getTeleportStartYSpeed() {
    return m_teleportStartYSpeed;
  }
  inline FixedPoint8 getTeleportStopPosY() {
    return m_teleportStopPosY;
  }
  Tile getTile(int a_x, int a_y);
  inline int getWidth() {
    return m_width;
  }
  bool isEnteringInvalidScreen(int a_x, int a_y);
  bool isLeavingScreenBottom(int a_y);
  bool isLeavingScreenTop(int a_y);
  void setCurrentBoundaryByPosition(int a_x, int a_y);
};

#endif
