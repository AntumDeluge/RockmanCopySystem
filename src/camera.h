#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

class Camera {
  private:
  static const int kWidth = 256;
  static const int kHeight = 240;
  SDL_Rect m_angleOfView, m_currentBoundary;
  bool areColliding(SDL_Rect* a_pBoxA, SDL_Rect* a_pBoxB);

  public:
  Camera();
  const SDL_Rect& getAngleOfView();
  int getScreenMappedXCoordinate(int a_x);
  int getScreenMappedYCoordinate(int a_y);
  bool isViewingSubject(SDL_Rect* a_pSubjectBox);
  void move(int a_xOffset, int a_yOffset);
  void setCurrentBoundary(SDL_Rect a_pBoundary);
  void update(const SDL_Rect& a_centeringBox);
};

#endif
