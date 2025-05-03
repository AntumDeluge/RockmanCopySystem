#ifndef FONT_H
#define FONT_H

#include "bitmap.h"
#include "inidictionary.h"

class Font {
  private:
  // Prevent copying because a copied instance
  // could free the loaded Bitmap on destruction.
  Font(const Font&);
  Font& operator=(const Font&);
  int m_characterHeight, m_characterWidth;
  Bitmap m_characterSetBitmap;
  IniDictionary m_characterSetBitmapMap;

  public:
  Font(const char* a_pIniFile);
  ~Font();
  void drawNumberOnScreen(int a_number, int a_x, int a_y, int a_nDigits);
  void drawTextOnBitmap(std::string a_text, Bitmap& a_destinationBitmap, int a_x = 0, int a_y = 0);
  void drawTextOnScreen(std::string a_text, int a_x, int a_y);
  inline int getCharacterHeight() {
    return m_characterHeight;
  }
  inline int getCharacterWidth() {
    return m_characterWidth;
  }
};

#endif
