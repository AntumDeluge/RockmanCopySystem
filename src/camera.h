#ifndef CAMERA_H
#define CAMERA_H

#include "rect.h"

class Camera {
  private:
  static const int kWidth = 256;
  static const int kHeight = 240;
  Rect m_angleOfView, m_currentBoundary;
  bool areColliding(Rect* a_pBoxA, Rect* a_pBoxB);

  public:
  Camera();
  const Rect& getAngleOfView();
  int getScreenMappedXCoordinate(int a_x);
  int getScreenMappedYCoordinate(int a_y);
  bool isViewingSubject(Rect* a_pSubjectBox);
  void move(int a_xOffset, int a_yOffset);
  void setCurrentBoundary(Rect a_pBoundary);
  void update(const Rect& a_centeringBox);
};

#endif
