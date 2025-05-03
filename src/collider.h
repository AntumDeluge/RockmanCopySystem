#ifndef COLLIDER_H
#define COLLIDER_H

#include "SDL.h"

class Collider {
  private:
  SDL_Rect* m_pBoundary;
  bool areColliding(SDL_Rect* a_pBoxA, SDL_Rect* a_pBoxB);

  public:
  Collider(SDL_Rect* a_pBoundary);
  bool collidesWith(Collider* a_collider);
  SDL_Rect getBoundary();
};

#endif
