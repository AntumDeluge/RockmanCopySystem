#ifndef COLLIDER_H
#define COLLIDER_H

#include "rect.h"

class Collider {
  private:
  Rect* m_pBoundary;
  bool areColliding(Rect* a_pBoxA, Rect* a_pBoxB);

  public:
  Collider(Rect* a_pBoundary);
  bool collidesWith(Collider* a_collider);
  Rect getBoundary();
};

#endif
