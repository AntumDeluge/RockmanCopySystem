#ifndef ENEMY_H
#define ENEMY_H

#include "SDL.h"
#include "collider.h"

class Camera;
class Rockman;
class Sprite;

class Enemy {
  private:
  SDL_Rect m_boundingBox;
  Collider m_collider;
  Sprite* m_pSprite;

  public:
  Enemy();
  ~Enemy();
  void draw(Camera& a_camera);
  Collider getCollider();
  void hit(Rockman* a_pRockman);
  void update();
};

#endif
