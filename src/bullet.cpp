#include "bullet.h"


Bullet::Bullet()
  : m_x(0)
  , m_y(0)
  , m_direction(Direction::None)
  , m_active(false)
{}

bool Bullet::activate(int a_x, int a_y, Direction::type a_direction) {
  if (!m_active) {
    m_x = a_x;
    m_y = a_y;
    m_direction = a_direction;
    m_active = true;
  }
  else {
    return false;
  }
  return true;
}

void Bullet::deactivate() {
  m_active = false;
}

void Bullet::draw(SDL_Surface* a_pBitmap) {
  if (m_active) {
    gameEngine->blitToScreen(m_x, m_y, a_pBitmap);
  }
}

void Bullet::update() {
  if (m_active) {
    SDL_Rect camera = gameEngine->getCamera();
    if (m_direction == Direction::Right) {
      m_x += kXDelta;
      int rightBoundaryX = camera.x + kScreenWidth;
      if (m_x >= rightBoundaryX) {
        m_active = false;
      }
    }
    else if (m_direction == Direction::Left) {
      m_x -= kXDelta;
      int leftBoundaryX = camera.x - kWidth;
      if (m_x <= leftBoundaryX) {
        m_active = false;
      }
    }
  }
}
