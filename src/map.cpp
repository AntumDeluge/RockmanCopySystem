#include "map.h"
#include "camera.h"
#include "servicelocator.h"
#include <cstdio>
#include <fstream>


Map::Map(const char* a_pMapName, IniDictionary& a_iniDictionary)
  : m_width(0)
  , m_height(0)
  , m_nBoundaries(0)
  , m_currentBoundaryIndex(0)
  , m_nTiles(0)
  , m_nTileSetBitmapAreas(0)
  , m_nScreens(0)
  , m_nScreensHorizontal(0)
  , m_nScreensVertical(0)
  , m_scrollStartPosTop(0)
  , m_scrollStartPosBottom(kScreenHeight)
  , m_teleportStopPosY(0)
  , m_teleportStartYSpeed(0)
  , m_pBoundaries(0)
  , m_pTileSetBitmapAreas(0)
  , m_ppScreens(0)
{
  const char* tileSheetFilename = a_iniDictionary.getCStringValue("tileSheet", a_pMapName, "empty");
  VideoService& videoService = ServiceLocator::getVideoService();
  videoService.loadBitmap(m_tileSetBitmap, tileSheetFilename);
  const char* mapFilename = a_iniDictionary.getCStringValue("tileMap", a_pMapName, "empty");
  std::ifstream map(mapFilename, std::ifstream::binary);
#ifdef DEBUG
  std::printf("Map::load: mapFilename: %s\n", mapFilename);
#endif
  int nTilesWide1 = map.get();
  int nTilesWide2 = map.get();
  int nTilesWide = nTilesWide1 * 256 + nTilesWide2;
  int nTilesHigh1 = map.get();
  int nTilesHigh2 = map.get();
  int nTilesHigh = nTilesHigh1 * 256 + nTilesHigh2;
  m_nTileSetBitmapAreas = map.get();
  // Bitmap area zero is an empty tile that isn't included in the tile set.
  // Therefore, it doesn't count towards the total bitmap area count.
  m_nTileSetBitmapAreas--;
  m_nTiles = nTilesWide * nTilesHigh;
  m_width = nTilesWide * Tile::kWidth;
  m_height = nTilesHigh * Tile::kHeight;
  m_pTileSetBitmapAreas = new Rect[m_nTileSetBitmapAreas];
  for (int iTileSetBitmapArea = 0; iTileSetBitmapArea < m_nTileSetBitmapAreas; iTileSetBitmapArea++) {
    m_pTileSetBitmapAreas[iTileSetBitmapArea].x = 0;
    m_pTileSetBitmapAreas[iTileSetBitmapArea].y = iTileSetBitmapArea * Tile::kHeight;
    m_pTileSetBitmapAreas[iTileSetBitmapArea].w = Tile::kWidth;
    m_pTileSetBitmapAreas[iTileSetBitmapArea].h = Tile::kHeight;
  }
  m_nScreensHorizontal = nTilesWide / knTilesPerRowInScreen;
  m_nScreensVertical = nTilesHigh / knTilesPerColumnInScreen;
  m_nScreens = m_nScreensHorizontal * m_nScreensVertical;
  m_ppScreens = new Tile*[m_nScreens];
  for (int iScreen = 0; iScreen < m_nScreens; iScreen++) {
    m_ppScreens[iScreen] = 0;
  }
  for (int iTileRow = 0; iTileRow < nTilesHigh; iTileRow++) {
    for (int iTileColumn = 0; iTileColumn < nTilesWide; iTileColumn+= knTilesPerRowInScreen) {
      int iNextTileSetBitmapArea = map.peek();
      // Bitmap area zero is an empty tile. If we encounter one, it means
      // we're at the beginning of a row of tiles of an empty screen.
      if (iNextTileSetBitmapArea == 0) {
        map.ignore(knTilesPerRowInScreen * knTileBytes);
      }
      else {
        int iCurrentScreen = iTileRow / knTilesPerColumnInScreen * m_nScreensHorizontal + iTileColumn / knTilesPerRowInScreen;
        // Determine if we're on the first row of a screen.
        int iCurrentScreenRow = iTileRow % knTilesPerColumnInScreen;
        if (iCurrentScreenRow == 0) {
          m_ppScreens[iCurrentScreen] = new Tile[knTilesPerRowInScreen * knTilesPerColumnInScreen];
        }
        int iCurrentScreenColumn = iTileColumn % knTilesPerRowInScreen;
        int iCurrentTile = iCurrentScreenRow * knTilesPerRowInScreen + iCurrentScreenColumn;
        for (int iScreenColumnTile = 0; iScreenColumnTile < knTilesPerRowInScreen; iScreenColumnTile++) {
          int iCurrentTileSetBitmapArea = map.get();
          // Bitmap area zero is an empty tile that isn't included in the tile set.
          // Therefore, to map the read integer to the correct internal bitmap area,
          // the value has to be decreased by one.
          iCurrentTileSetBitmapArea--;
          int tileTypeInt = map.get();
          Tile::Type::type tileType = (Tile::Type::type)tileTypeInt;
          m_ppScreens[iCurrentScreen][iCurrentTile].bitmapAreaId = iCurrentTileSetBitmapArea;
          m_ppScreens[iCurrentScreen][iCurrentTile].type = tileType;
          m_ppScreens[iCurrentScreen][iCurrentTile].x = (iTileColumn + iScreenColumnTile) * Tile::kWidth;
          m_ppScreens[iCurrentScreen][iCurrentTile].y = iTileRow * Tile::kHeight;
          iCurrentTile++;
        }
      }
    }
  }
  map.close();
  m_nBoundaries = a_iniDictionary.getIntValue("boundaries", a_pMapName, 0);
  m_pBoundaries = new Rect[m_nBoundaries];
  for (int iBoundary = 0; iBoundary < m_nBoundaries; iBoundary++) {
    char indexString[2];
    std::sprintf(indexString, "%d", iBoundary);
    std::string xKey("boundary");
    xKey += indexString;
    std::string yKey(xKey);
    std::string wKey(xKey);
    xKey += "x";
    yKey += "y";
    wKey += "w";
    Rect boundary;
    boundary.x = a_iniDictionary.getIntValue(xKey.c_str(), a_pMapName, 0);
    boundary.y = a_iniDictionary.getIntValue(yKey.c_str(), a_pMapName, 0);
    boundary.w = a_iniDictionary.getIntValue(wKey.c_str(), a_pMapName, 0);
    boundary.h = kScreenHeight;
    m_pBoundaries[iBoundary] = boundary;
  }
  int teleportStartYSpeedFraction = a_iniDictionary.getIntValue("teleportStartYSpeedFraction", a_pMapName, 0);
  m_teleportStartYSpeed = FixedPoint8(0, teleportStartYSpeedFraction);
  int screenRelativeTeleportStopPosY = a_iniDictionary.getIntValue("teleportStopPosY", a_pMapName, 0);
  m_teleportStopPosY = m_pBoundaries[0].y + screenRelativeTeleportStopPosY;
}

Map::~Map() {
  for (int iScreen = 0; iScreen < m_nScreens; iScreen++) {
    if (m_ppScreens[iScreen] != 0) {
      delete [] m_ppScreens[iScreen];
    }
  }
  delete [] m_ppScreens;
  delete [] m_pTileSetBitmapAreas;
  VideoService& videoService = ServiceLocator::getVideoService();
  videoService.unloadBitmap(m_tileSetBitmap);
}

void Map::draw(Camera& a_camera) {
  VideoService& videoService = ServiceLocator::getVideoService();
  const Rect& cameraAngleOfView = a_camera.getAngleOfView();
  int screen = cameraAngleOfView.y / kScreenHeight * m_nScreensHorizontal + cameraAngleOfView.x / kScreenWidth;
  for (int iTile = 0; iTile < knTilesPerRowInScreen * knTilesPerColumnInScreen; iTile++) {
    int x = a_camera.getScreenMappedXCoordinate(m_ppScreens[screen][iTile].x);
    int y = a_camera.getScreenMappedYCoordinate(m_ppScreens[screen][iTile].y);
    videoService.copyToScreen(m_tileSetBitmap, x, y, &m_pTileSetBitmapAreas[m_ppScreens[screen][iTile].bitmapAreaId]);
  }
  // Check if the camera is positioned on two horizontal screens at once.
  int secondScreenViewableX = cameraAngleOfView.x % kScreenWidth;
  if (secondScreenViewableX > 0) {
    screen++;
    for (int iTile = 0; iTile < knTilesPerRowInScreen * knTilesPerColumnInScreen; iTile++) {
      int x = a_camera.getScreenMappedXCoordinate(m_ppScreens[screen][iTile].x);
      int y = a_camera.getScreenMappedYCoordinate(m_ppScreens[screen][iTile].y);
      videoService.copyToScreen(m_tileSetBitmap, x, y, &m_pTileSetBitmapAreas[m_ppScreens[screen][iTile].bitmapAreaId]);
    }
  }
  else {
    // Check if the camera is positioned on two vertical screens at once.
    int secondScreenViewableY = cameraAngleOfView.y % kScreenHeight;
    if (secondScreenViewableY > 0) {
      screen += m_nScreensHorizontal;
      for (int iTile = 0; iTile < knTilesPerRowInScreen * knTilesPerColumnInScreen; iTile++) {
        int x = a_camera.getScreenMappedXCoordinate(m_ppScreens[screen][iTile].x);
        int y = a_camera.getScreenMappedYCoordinate(m_ppScreens[screen][iTile].y);
        videoService.copyToScreen(m_tileSetBitmap, x, y, &m_pTileSetBitmapAreas[m_ppScreens[screen][iTile].bitmapAreaId]);
      }
    }
  }
}

Rect Map::getCurrentBoundary() {
  return m_pBoundaries[m_currentBoundaryIndex];
}

LadderSonar Map::getLadderSonar(int a_x, int a_y, int a_yNextFrame, int a_boundingBoxYOffset, bool a_movingUp, bool a_useBoundingBoxOffset) {
  LadderSonar ladderSonar = { false, false, false, false, false };
  int y = a_y + LadderSonar::kAtFeetOffsetY;
  Tile detectedTile = getTile(a_x, y);
  if (detectedTile.type == Tile::Type::Climbable) {
    ladderSonar.atFeet = true;
  }
  y = a_y + LadderSonar::kBehindEyesOffsetY;
  detectedTile = getTile(a_x, y);
  if (detectedTile.type == Tile::Type::Climbable) {
    ladderSonar.behindEyes = true;
  }
  if (!a_useBoundingBoxOffset) {
    y = a_y + LadderSonar::kAtHeadTopOffsetY;
  }
  else {
    y = a_y + a_boundingBoxYOffset + LadderSonar::kAtHeadTopBoundingBoxOffsetY;
  }
  detectedTile = getTile(a_x, y);
  if (detectedTile.type == Tile::Type::Climbable) {
    ladderSonar.atHeadTop = true;
  }
  if (a_movingUp) {
    if (!a_useBoundingBoxOffset) {
      y = a_yNextFrame + LadderSonar::kAtHeadTopOffsetY;
    }
    else {
      y = a_yNextFrame + a_boundingBoxYOffset + LadderSonar::kAtHeadTopBoundingBoxOffsetY;
    }
    detectedTile = getTile(a_x, y);
    if (detectedTile.type == Tile::Type::Climbable) {
      ladderSonar.atHeadTopNextFrame = true;
    }
  }
  else {
    y = a_yNextFrame + LadderSonar::kBelowFeetOffsetY;
    detectedTile = getTile(a_x, y);
    if (detectedTile.type == Tile::Type::Climbable) {
      ladderSonar.belowFeetNextFrame = true;
    }
  }
  return ladderSonar;
}

FixedPoint8 Map::getTeleportStartPosX() {
  FixedPoint8 x = m_pBoundaries[0].x + kTeleportStartPosXInt;
  return x;
}

FixedPoint8 Map::getTeleportStartPosY() {
  int yInt = m_pBoundaries[0].y + kTeleportStartPosYInt;
  FixedPoint8 y = FixedPoint8(yInt, kTeleportStartPosYFraction);
  return y;
}

int Map::getMaxXDelta(Rect a_boundingBox, Direction::type a_direction) {
  int maxXDelta = 0;
  int collisionX = 0;
  if (a_direction == Direction::Right) {
    collisionX = a_boundingBox.x + a_boundingBox.w + Tile::kWidth;
    maxXDelta = Tile::kWidth - 1 - (collisionX % Tile::kWidth);
  }
  else if (a_direction == Direction::Left) {
    collisionX = a_boundingBox.x - Tile::kWidth;
    if (collisionX < 0) {
      return a_boundingBox.x;
    }
    maxXDelta = collisionX % Tile::kWidth;
  }
  // Get the tile to the bottom left or right.
  int collisionY = a_boundingBox.y + a_boundingBox.h;
  Tile sideTile = getTile(collisionX, collisionY);
  if (sideTile.type == Tile::Type::Collidable) {
    return maxXDelta;
  }
  // Get the tile to the middle left or right.
  collisionY = a_boundingBox.y + (a_boundingBox.h / 2);
  sideTile = getTile(collisionX, collisionY);
  if (sideTile.type == Tile::Type::Collidable) {
    return maxXDelta;
  }
  // Get the tile to the top left or right.
  collisionY = a_boundingBox.y;
  sideTile = getTile(collisionX, collisionY);
  if (sideTile.type == Tile::Type::Collidable) {
    return maxXDelta;
  }
  return Tile::kWidth;
}

int Map::getMaxYDelta(Rect a_boundingBox, Direction::type a_direction) {
  int maxYDelta = 0;
  int collisionY = 0;
  int currentY = a_boundingBox.y + a_boundingBox.h;
  if (a_direction == Direction::Down) {
    collisionY = a_boundingBox.y + a_boundingBox.h + Tile::kHeight;
    maxYDelta = Tile::kHeight - 1 - (collisionY % Tile::kHeight);
  }
  else if (a_direction == Direction::Up) {
    collisionY = a_boundingBox.y - Tile::kHeight;
    maxYDelta = collisionY % Tile::kHeight;
  }
  int collisionX = a_boundingBox.x + a_boundingBox.w;
  Tile topbottomRightTile = getTile(collisionX, collisionY);
  if (topbottomRightTile.type == Tile::Type::Collidable) {
    return maxYDelta;
  }
  // A ladder is not collidable, except for its top.
  // Check if Mega Man is above a ladder.
  else if (topbottomRightTile.type == Tile::Type::Climbable && a_direction == Direction::Down) {
    Tile currentTile = getTile(collisionX, currentY);
    if (currentTile.type != Tile::Type::Climbable) {
      return maxYDelta;
    }
  }
  collisionX = a_boundingBox.x;
  Tile topbottomLeftTile = getTile(collisionX, collisionY);
  if (topbottomLeftTile.type == Tile::Type::Collidable) {
    return maxYDelta;
  }
  // A ladder is not collidable, except for its top.
  // Check if Mega Man is above a ladder.
  else if (topbottomLeftTile.type == Tile::Type::Climbable && a_direction == Direction::Down) {
    Tile currentTile = getTile(collisionX, currentY);
    if (currentTile.type != Tile::Type::Climbable) {
      return maxYDelta;
    }
  }
  return Tile::kHeight;
}

Tile Map::getTile(int a_x, int a_y) {
  if (a_x < 0) {
    a_x = 0;
  }
  if (a_y < 0) {
    a_y = 0;
  }
  int iCurrentScreen = (a_y / kScreenHeight) * m_nScreensHorizontal + (a_x / kScreenWidth);
  int iCurrentScreenRow = a_y % kScreenHeight / Tile::kHeight;
  int iCurrentScreenColumn = a_x % kScreenWidth / Tile::kWidth;
  int iTile = iCurrentScreenRow * knTilesPerRowInScreen + iCurrentScreenColumn;
  Tile tile;
  if (iCurrentScreen < m_nScreens && m_ppScreens[iCurrentScreen]) {
    tile = m_ppScreens[iCurrentScreen][iTile];
  }
  else {
    // Requested tile is not in a non-empty screen.
    tile.bitmapAreaId = 0;
    tile.type = Tile::Type::Collidable;
    tile.x = a_x - (a_x % Tile::kWidth);
    tile.y = a_y - (a_y % Tile::kHeight);
  }
  // Tiles to the left and right of the current boundary should be collidable.
  // Those above and below of the current boundary, however, should not.
  if (a_y < m_pBoundaries[m_currentBoundaryIndex].y
      || (a_y > m_pBoundaries[m_currentBoundaryIndex].y + kScreenHeight)) {
    if (tile.type == Tile::Type::Collidable) {
      tile.type = Tile::Type::Background;
    }
  }
  return tile;
}

bool Map::isEnteringInvalidScreen(int a_x, int a_y) {
  int bottomScreenY = a_y;
  int screenRelativeY = a_y - m_pBoundaries[m_currentBoundaryIndex].y;
  if (screenRelativeY < kScreenHeight) {
    int offsetToBottomScreen = kScreenHeight - screenRelativeY;
    bottomScreenY += offsetToBottomScreen;
  }
  int screenIndex = (bottomScreenY / kScreenHeight) * m_nScreensHorizontal + (a_x / kScreenWidth);
  if (screenIndex < m_nScreens && m_ppScreens[screenIndex]) {
    return false;
  }
  return true;
}

bool Map::isLeavingScreenBottom(int a_y) {
  int screenRelativeY = a_y - m_pBoundaries[m_currentBoundaryIndex].y;
  if (screenRelativeY >= m_scrollStartPosBottom) {
    return true;
  }
  return false;
}

bool Map::isLeavingScreenTop(int a_y) {
  int screenRelativeY = a_y - m_pBoundaries[m_currentBoundaryIndex].y;
  if (screenRelativeY <= m_scrollStartPosTop) {
    return true;
  }
  return false;
}

void Map::setCurrentBoundaryByPosition(int a_x, int a_y) {
#ifdef DEBUG
  std::printf("%s%i%c%i%c", "getBoundaryByPosition: ", a_x, 'x', a_y, '\n');
#endif
  for (int iBoundary = 0; iBoundary < m_nBoundaries; iBoundary++) {
#ifdef DEBUG
  std::printf("Boundary %i\n", iBoundary);
#endif
    if (a_x >= m_pBoundaries[iBoundary].x
        && a_x < m_pBoundaries[iBoundary].x + m_pBoundaries[iBoundary].w
        && a_y >= m_pBoundaries[iBoundary].y
        && a_y < m_pBoundaries[iBoundary].y + m_pBoundaries[iBoundary].h) {
      m_currentBoundaryIndex = iBoundary;
      return;
    }
  }
}

void Map::setScrollStartPositions(int a_scrollStartPosTop, int a_scrollStartPosBottom) {
  m_scrollStartPosTop = a_scrollStartPosTop;
  m_scrollStartPosBottom = a_scrollStartPosBottom;
}
