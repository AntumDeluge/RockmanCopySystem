#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include "font.h"

class DebugInfo {
  private:
  Font m_font;

  public:
  DebugInfo();
  void draw(int a_x, int a_y, int a_xSpeed, int a_ySpeed, int a_ladderSonarInt);
};

#endif
