#ifndef FONT_H
#define FONT_H

#include "SDL.h"
#include "inidictionary.h"
class GameEngine;

class Font {
  private:
  // Prevent copying because a copied instance
  // could free the loaded SDL_Surface on destruction.
  Font(const Font&);
  Font& operator=(const Font&);
  int m_characterHeight, m_characterWidth;
  IniDictionary m_characterSetBitmapMap;
  GameEngine* m_pGameEngine;
  SDL_Surface* m_characterSetBitmap;
  void drawText(std::string& a_text, SDL_Surface* a_pDestinationBitmap, int a_x, int a_y, bool a_toScreen);

  public:
  Font(const char* a_pIniFile, GameEngine* a_pGameEngine);
  ~Font();
  void drawNumber(int a_number, int a_x, int a_y, int a_nDigits);
  void drawText(std::string a_text, int a_x, int a_y);
  void drawText(std::string a_text, SDL_Surface* a_pDestinationBitmap, int a_x = 0, int a_y = 0);
  inline int getCharacterHeight() {
    return m_characterHeight;
  }
  inline int getCharacterWidth() {
    return m_characterWidth;
  }
};

#endif
