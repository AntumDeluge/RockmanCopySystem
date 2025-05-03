#ifndef TILE_H
#define TILE_H

struct Tile {
  static const int kWidth = 16;
  static const int kHeight = 16;
  struct Type {
    enum type {
      Background = 0,
      Collidable = 1,
      Climbable = 2
    };
  };
  int bitmapAreaId;
  Type::type type;
  int x;
  int y;
};

#endif
