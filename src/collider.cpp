#include "collider.h"


Collider::Collider(SDL_Rect* a_pBoundary)
  : m_pBoundary(a_pBoundary)
{}

bool Collider::collidesWith(Collider* a_pCollider) {
  SDL_Rect collideeBoundary = a_pCollider->getBoundary();
  bool collides = gameEngine->areColliding(m_pBoundary, &collideeBoundary);
  return collides;
}

SDL_Rect Collider::getBoundary() {
  return *m_pBoundary;
}
