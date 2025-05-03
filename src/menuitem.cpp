#include "menuitem.h"
#include "globals.h"


MenuItem::MenuItem()
  : m_x(0)
  , m_y(0)
{}

MenuItem::MenuItem(const char* a_pCaption, const char* a_pValue, int a_x, int a_y)
  : m_x(a_x)
  , m_y(a_y)
  , m_caption(a_pCaption)
  , m_value(a_pValue)
{}

void MenuItem::draw() {
  Font& font = gameEngine->getFont();
  font.drawText(m_caption, m_x, m_y);
}

std::string MenuItem::getValue() {
  return m_value;
}

int MenuItem::getX() {
  return m_x;
}

int MenuItem::getY() {
  return m_y;
}
