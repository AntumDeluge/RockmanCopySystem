#ifndef LADDERSONAR_H
#define LADDERSONAR_H

struct LadderSonar {
  static const int kBelowFeetOffsetY = 12;
  static const int kAtFeetOffsetY = 11;
  static const int kBehindEyesOffsetY = -4;
  static const int kAtHeadTopOffsetY = -12;
  static const int kAtHeadTopBoundingBoxOffsetY = 0;
  bool belowFeetNextFrame;
  bool atFeet;
  bool behindEyes;
  bool atHeadTop;
  bool atHeadTopNextFrame;
  int getAsInt() {
    int ladderSonarInt = 0;
    if (belowFeetNextFrame) {
      ladderSonarInt += 1;
    }
    if (atFeet) {
      ladderSonarInt += 2;
    }
    if (behindEyes) {
      ladderSonarInt += 4;
    }
    if (atHeadTop) {
      ladderSonarInt += 8;
    }
    if (atHeadTopNextFrame) {
      ladderSonarInt += 16;
    }
    return ladderSonarInt;
  }
};

#endif
