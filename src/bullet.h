#ifndef BULLET_H
#define BULLET_H

#include "SDL.h"
#include "direction.h"

class Camera;

class Bullet {
  private:
  static const int kWidth = 8;
  static const int kHeight = 8;
  static const int kXDelta = 4;
  Direction::type m_direction;
  bool m_active;
  SDL_Rect m_boundingBox;

  public:
  Bullet();
  bool activate(int a_x, int a_y, Direction::type a_direction);
  void deactivate();
  void draw(SDL_Surface* a_pBitmap, Camera& a_camera);
  void update(Camera& a_camera);
};

#endif
