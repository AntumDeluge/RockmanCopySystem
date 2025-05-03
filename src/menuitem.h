#ifndef MENUITEM_H
#define MENUITEM_H

#include "font.h"
#include <string>

class MenuItem {
  private:
  int m_x, m_y;
  std::string m_caption;
  std::string m_value;

  public:
  MenuItem();
  MenuItem(const char* a_pCaption, const char* a_pValue, int a_x, int a_y);
  void draw(Font& a_font);
  std::string getValue();
  int getX();
  int getY();
};

#endif
