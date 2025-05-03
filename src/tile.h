#ifndef TILE_H
#define TILE_H

#include "globals.h"

struct Tile {
  static const int kWidth = 16;
  static const int kHeight = 16;
  static const int kLadderType = 4;
  int type;
  bool collidable;
  int x;
  int y;
};

#endif
