#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "inidictionary.h"

struct Animation;

class Sprite {
  private:
  typedef void (Sprite::*AnimationOrderHandler)();
  int m_iCurrentAnimation;
  int m_iCurrentFrame;
  int m_duration;
  int m_step;
  int m_nAnimations;
  int m_nBitmaps;
  int m_xOffset;
  int m_yOffset;
  Animation* m_pAnimations;
  AnimationOrderHandler m_currentAnimationOrderHandler;
  SDL_Surface** m_ppBitmaps;
  SDL_Rect* m_pBitmapAreas;

  public:
  Sprite(IniDictionary& a_iniDictionary);
  ~Sprite();
  void animateForward();
  void animateForwardBackward();
  void draw(int a_x, int a_y, int a_iBitmap);
  void resetCurrentAnimationFrameDuration();
  void setCurrentAnimationIndex(int a_iAnimation);
  void setForwardBackwardAnimationOrder();
  void update();
};

#endif
