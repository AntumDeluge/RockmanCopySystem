#include "font.h"
#include "SDL.h"
#include "servicelocator.h"


Font::Font(const char* a_pIniFile)
  : m_characterHeight(0)
  , m_characterWidth(0)
  , m_characterSetBitmapMap(a_pIniFile)
  , m_characterSetBitmap(0)
{
  const char* pCharacterSetBitmapFile = m_characterSetBitmapMap.getCStringValue("sheet", "Properties", "empty");
  VideoService& videoService = ServiceLocator::getVideoService();
  m_characterSetBitmap = videoService.loadSurface(pCharacterSetBitmapFile);
  m_characterHeight = m_characterSetBitmapMap.getIntValue("characterHeight", "Properties", 0);
  m_characterWidth = m_characterSetBitmapMap.getIntValue("characterWidth", "Properties", 0);
}

Font::~Font() {
  VideoService& videoService = ServiceLocator::getVideoService();
  videoService.unloadSurface(m_characterSetBitmap);
}

void Font::drawNumber(int a_number, int a_x, int a_y, int a_nDigits) {
  VideoService& videoService = ServiceLocator::getVideoService();
  SDL_Rect characterSetBitmapArea = { 0, 0, m_characterHeight, m_characterWidth };
  if (a_number < 0) {
    characterSetBitmapArea.y = m_characterSetBitmapMap.getIntValue("-", "Characters", 0);
    videoService.blitUiToScreen(m_characterSetBitmap, a_x, a_y, &characterSetBitmapArea);
    a_number *= -1;
  }
  a_x += m_characterWidth;
  for (int iDigit = a_nDigits; iDigit > 0; iDigit--) {
    int divider = 1;
    int iExponent = iDigit;
    while (iExponent > 1) {
      divider *= 10;
      iExponent--;
    }
    int digit = a_number / divider;
    characterSetBitmapArea.y = digit * m_characterHeight;
    videoService.blitUiToScreen(m_characterSetBitmap, a_x, a_y, &characterSetBitmapArea);
    a_number -= (digit * divider);
    a_x += m_characterWidth;
  }
}

void Font::drawText(std::string a_text, int a_x, int a_y) {
  drawText(a_text, 0, a_x, a_y, true);
}

void Font::drawText(std::string a_text, SDL_Surface* a_pDestinationBitmap, int a_x, int a_y) {
  drawText(a_text, a_pDestinationBitmap, a_x, a_y, false);
}

void Font::drawText(std::string& a_text, SDL_Surface* a_pDestinationBitmap, int a_x, int a_y, bool a_toScreen) {
  VideoService& videoService = ServiceLocator::getVideoService();
  SDL_Rect characterSetBitmapArea = { 0, 0, m_characterHeight, m_characterWidth };
  for (unsigned int iCharacter = 0; iCharacter < a_text.length(); iCharacter++) {
    if (a_text[iCharacter] == ' ') {
      characterSetBitmapArea.y = m_characterSetBitmapMap.getIntValue("space", "Characters", 0);
    }
    else {
      const char character[2] = { a_text[iCharacter], 0 };
      characterSetBitmapArea.y = m_characterSetBitmapMap.getIntValue(character, "Characters", 0);
    }
    if (a_toScreen) {
      videoService.blitToScreen(m_characterSetBitmap, a_x, a_y, &characterSetBitmapArea);
    }
    else {
      videoService.blitToSurface(m_characterSetBitmap, a_pDestinationBitmap, a_x, a_y, &characterSetBitmapArea);
    }
    a_x += m_characterWidth;
  }
}
