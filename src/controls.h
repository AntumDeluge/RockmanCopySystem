#ifndef CONTROLS_H
#define CONTROLS_H

#include "direction.h"

struct Controls {
  Direction::type horizontalDirection;
  Direction::type verticalDirection;
  Direction::type previousHorizontalDirection;
  Direction::type previousVerticalDirection;
  bool upDown;
  bool downDown;
  bool leftDown;
  bool rightDown;
  bool actionPressed;
  bool actionDown;
  bool actionReleased;
  bool jumpPressed;
  bool jumpDown;
  bool jumpReleased;
};

#endif
