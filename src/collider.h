#ifndef COLLIDER_H
#define COLLIDER_H

#include "globals.h"

class Collider {
  private:
  SDL_Rect* m_pBoundary;

  public:
  Collider(SDL_Rect* a_pBoundary);
  bool collidesWith(Collider* a_collider);
  SDL_Rect getBoundary();
};

#endif
