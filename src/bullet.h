#ifndef BULLET_H
#define BULLET_H

#include "globals.h"
#include "direction.h"

class Bullet {
  private:
  static const int kWidth = 8;
  static const int kXDelta = 4;
  int m_x;
  int m_y;
  Direction::type m_direction;
  bool m_active;

  public:
  Bullet();
  bool activate(int a_x, int a_y, Direction::type a_direction);
  void deactivate();
  void draw(SDL_Surface* a_pBitmap);
  void update();
};

#endif
