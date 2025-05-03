#include "collider.h"


Collider::Collider(Rect* a_pBoundary)
  : m_pBoundary(a_pBoundary)
{}

bool Collider::areColliding(Rect* a_pBoxA, Rect* a_pBoxB) {
  int leftBoxA = a_pBoxA->x;
  int rightBoxA = a_pBoxA->x + a_pBoxA->w;
  int topBoxA = a_pBoxA->y;
  int bottomBoxA = a_pBoxA->y + a_pBoxA->h;
  int leftBoxB = a_pBoxB->x;
  int rightBoxB = a_pBoxB->x + a_pBoxB->w;
  int topBoxB = a_pBoxB->y;
  int bottomBoxB = a_pBoxB->y + a_pBoxB->h;
  if (bottomBoxA <= topBoxB) {
    return false;
  }
  if (topBoxA >= bottomBoxB) {
    return false;
  }
  if (rightBoxA <= leftBoxB) {
    return false;
  }
  if (leftBoxA >= rightBoxB) {
    return false;
  }
  return true;
}

bool Collider::collidesWith(Collider* a_pCollider) {
  Rect collideeBoundary = a_pCollider->getBoundary();
  bool collides = areColliding(m_pBoundary, &collideeBoundary);
  return collides;
}

Rect Collider::getBoundary() {
  return *m_pBoundary;
}
