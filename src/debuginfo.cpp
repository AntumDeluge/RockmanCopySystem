#include "debuginfo.h"
#include "globals.h"


void DebugInfo::draw(int a_x, int a_y, int a_xSpeed, int a_ySpeed, int a_ladderSonarInt) {
  int x = a_x >> 8;
  x = x % 256;
  int xFractions = a_x & 255;
  int y = a_y >> 8;
  y = y % 240;
  int yFractions = a_y & 255;
  int xSpeed = a_xSpeed >> 8;
  int xSpeedFractions = a_xSpeed & 255;
  int ySpeed = a_ySpeed >> 8;
  int ySpeedFractions = a_ySpeed & 255;
  Font& font = gameEngine->getFont();
  font.drawNumber(x, 8, 8, 3);
  font.drawNumber(xFractions, 40, 8, 3);
  font.drawNumber(y, 8, 16, 3);
  font.drawNumber(yFractions, 40, 16, 3);
  font.drawNumber(xSpeed, 16, 24, 2);
  font.drawNumber(xSpeedFractions, 40, 24, 3);
  font.drawNumber(ySpeed, 16, 32, 2);
  font.drawNumber(ySpeedFractions, 40, 32, 3);
  font.drawNumber(a_ladderSonarInt, 16, 48, 2);
}
