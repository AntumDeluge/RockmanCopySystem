#ifndef MAP_H
#define MAP_H

#include "bitmap.h"
#include "direction.h"
#include "fixedpoint8.h"
#include "inidictionary.h"
#include "laddersonar.h"
#include "rect.h"
#include "tile.h"

class Camera;

class Map {
  private:
  static const int kScreenHeight = 240;
  static const int kScreenWidth = 256;
  static const int knTilesPerColumnInScreen = kScreenHeight / Tile::kWidth;
  static const int knTilesPerRowInScreen = kScreenWidth / Tile::kWidth;
  static const int kTeleportStartPosXInt = kScreenWidth / 2;
  static const int kTeleportStartPosYInt = 4;
  static const int kTeleportStartPosYFraction = 248;
  static const int knTileBytes = 2;
  int m_width, m_height;
  int m_nBoundaries, m_currentBoundaryIndex;
  int m_nTiles, m_nTileSetBitmapAreas;
  int m_nScreens, m_nScreensHorizontal, m_nScreensVertical;
  int m_scrollStartPosTop, m_scrollStartPosBottom;
  int m_teleportStopPosY;
  Bitmap m_tileSetBitmap;
  FixedPoint8 m_teleportStartYSpeed;
  Rect* m_pBoundaries;
  Rect* m_pTileSetBitmapAreas;
  Tile** m_ppScreens;

  public:
  Map(const char* a_pMapName, IniDictionary& a_iniDictionary);
  ~Map();
  void draw(Camera& a_camera);
  Rect getCurrentBoundary();
  inline int getHeight() {
    return m_height;
  }
  int getMaxXDelta(Rect a_boundingBox, Direction::type a_direction);
  int getMaxYDelta(Rect a_boundingBox, Direction::type a_direction);
  LadderSonar getLadderSonar(int a_x, int a_y, int a_yNextFrame, int a_boundingBoxYOffset, bool a_movingUp, bool a_useBoundingBoxOffset);
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
  void setScrollStartPositions(int a_scrollStartPosTop, int a_scrollStartPosBottom);
};

#endif
