#include "bullet.h"
#include "camera.h"
#include "servicelocator.h"


Bullet::Bullet()
  : m_direction(Direction::None)
  , m_active(false)
  , m_boundingBox()
{
  m_boundingBox.w = kWidth;
  m_boundingBox.h = kHeight;
}

bool Bullet::activate(int a_x, int a_y, Direction::type a_direction) {
  if (!m_active) {
    m_boundingBox.x = a_x;
    m_boundingBox.y = a_y;
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

void Bullet::draw(SDL_Surface* a_pBitmap, Camera& a_camera) {
  if (m_active) {
    int x = a_camera.getScreenMappedXCoordinate(m_boundingBox.x);
    int y = a_camera.getScreenMappedYCoordinate(m_boundingBox.y);
    VideoService& videoService = ServiceLocator::getVideoService();
    videoService.blitToScreen(a_pBitmap, x, y);
  }
}

void Bullet::update(Camera& a_camera) {
  if (m_active) {
    if (m_direction == Direction::Right) {
      m_boundingBox.x += kXDelta;
    }
    else if (m_direction == Direction::Left) {
      m_boundingBox.x -= kXDelta;
    }
    m_active = a_camera.isViewingSubject(&m_boundingBox);
  }
}
