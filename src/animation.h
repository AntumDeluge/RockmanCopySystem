#ifndef ANIMATION_H
#define ANIMATION_H

#include "animationframe.h"

struct Animation {
  int nFrames;
  AnimationFrame* pFrames;
};

#endif
