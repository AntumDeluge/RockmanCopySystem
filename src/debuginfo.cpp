#include "debuginfo.h"

DebugInfo::DebugInfo()
  : m_font("font.ini")
{}

void DebugInfo::draw(int a_x, int a_y, int a_xSpeed, int a_ySpeed, int a_ladderSonarInt) {
  int x = a_x / 256;
  x = x % 256;
  int xFractions = a_x & 255;
  int y = a_y >> 8;
  y = y % 240;
  int yFractions = a_y & 255;
  int xSpeed = a_xSpeed / 256;
  int xSpeedFractions = a_xSpeed & 255;
  unsigned int ySpeedUnsigned = a_ySpeed;
  unsigned int ySpeed = (ySpeedUnsigned >> 8) & 255;
  unsigned int ySpeedFractions = ySpeedUnsigned & 255;
  m_font.drawNumber(x, 8, 8, 3);
  m_font.drawNumber(xFractions, 40, 8, 3);
  m_font.drawNumber(y, 8, 16, 3);
  m_font.drawNumber(yFractions, 40, 16, 3);
  m_font.drawNumber(xSpeed, 16, 24, 2);
  m_font.drawNumber(xSpeedFractions, 40, 24, 3);
  m_font.drawNumber(ySpeed, 8, 32, 3);
  m_font.drawNumber(ySpeedFractions, 40, 32, 3);
  m_font.drawNumber(a_ladderSonarInt, 16, 48, 2);
}
