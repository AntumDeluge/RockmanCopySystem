#include "camera.h"


Camera::Camera()
  : m_angleOfView()
  , m_currentBoundary()
{
  m_angleOfView.w = kWidth;
  m_angleOfView.h = kHeight;
}

const SDL_Rect& Camera::getAngleOfView() {
  return m_angleOfView;
}

int Camera::getScreenMappedXCoordinate(int a_x) {
  int screenMappedXCoordinate = a_x - m_angleOfView.x;
  return screenMappedXCoordinate;
}

int Camera::getScreenMappedYCoordinate(int a_y) {
  int screenMappedYCoordinate = a_y - m_angleOfView.y;
  return screenMappedYCoordinate;
}

bool Camera::isViewingSubject(SDL_Rect* a_pSubjectBox) {
  int leftBoxA = m_angleOfView.x;
  int rightBoxA = m_angleOfView.x + m_angleOfView.w;
  int topBoxA = m_angleOfView.y;
  int bottomBoxA = m_angleOfView.y + m_angleOfView.h;
  int leftBoxB = a_pSubjectBox->x;
  int rightBoxB = a_pSubjectBox->x + a_pSubjectBox->w;
  int topBoxB = a_pSubjectBox->y;
  int bottomBoxB = a_pSubjectBox->y + a_pSubjectBox->h;
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

void Camera::move(int a_xOffset, int a_yOffset) {
  m_angleOfView.x += a_xOffset;
  m_angleOfView.y += a_yOffset;
}

void Camera::setCurrentBoundary(SDL_Rect a_boundary) {
  m_currentBoundary = a_boundary;
  // necessary in case the boundary change is
  // not a result of scrolling
  m_angleOfView.y = m_currentBoundary.y;
}

void Camera::update(const SDL_Rect& a_centeringBox) {
  int x = a_centeringBox.x + (a_centeringBox.w / 2) - (kWidth / 2);
  int maxX = m_currentBoundary.x + m_currentBoundary.w - kWidth;
  if (x < m_currentBoundary.x) {
    x = m_currentBoundary.x;
  }
  else if (x > maxX) {
    x = maxX;
  }
  m_angleOfView.x = x;
}
