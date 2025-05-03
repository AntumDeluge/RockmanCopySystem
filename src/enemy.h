#ifndef ENEMY_H
#define ENEMY_H

#include "collider.h"
#include "rect.h"

class Camera;
class Rockman;
class Sprite;

class Enemy {
  private:
  Rect m_boundingBox;
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
