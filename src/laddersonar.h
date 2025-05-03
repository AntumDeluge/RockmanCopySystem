#ifndef LADDERSONAR_H
#define LADDERSONAR_H

struct LadderSonar {
  static const int kBelowFeetOffsetY = 12;
  static const int kAtFeetOffsetY = 11;
  static const int kBehindEyesOffsetY = -4;
  static const int kAtHeadTopOffsetY = -12;
  bool belowFeetNextFrame;
  bool atFeet;
  bool behindEyes;
  bool atHeadTop;
  bool atHeadTopNextFrame;
};

#endif
