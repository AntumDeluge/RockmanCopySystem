#include "sprite.h"
#include "animation.h"
#include "bitmap.h"
#include "rect.h"
#include "servicelocator.h"
#include <cstdio>
#include <string>


Sprite::Sprite(IniDictionary& a_iniDictionary)
  : m_iCurrentAnimation(0)
  , m_iCurrentFrame(0)
  , m_duration(0)
  , m_step(1)
  , m_nAnimations(0)
  , m_nBitmaps(0)
  , m_pAnimations(0)
  , m_currentAnimationOrderHandler(&Sprite::animateForward)
  , m_pBitmaps(0)
  , m_pBitmapAreas(0)
{
  m_nBitmaps = a_iniDictionary.getIntValue("amount", "Spritesheets", 0);
  m_pBitmaps = new Bitmap[m_nBitmaps];
  VideoService& videoService = ServiceLocator::getVideoService();
  for (int iBitmap = 0; iBitmap < m_nBitmaps; iBitmap++) {
    char indexString[3];
    std::sprintf(indexString, "%d", iBitmap);
    std::string key("spritesheet");
    key += indexString;
    std::string sheetFilename = a_iniDictionary.getCStringValue(key.c_str(), "Spritesheets", "empty");
    videoService.loadBitmap(m_pBitmaps[iBitmap], sheetFilename.c_str());
  }
  int nBitmapAreas = a_iniDictionary.getIntValue("amount", "Sprites", 0);
  m_pBitmapAreas = new Rect[nBitmapAreas];
  int width = a_iniDictionary.getIntValue("width", "Sprites", 0);
  int height = a_iniDictionary.getIntValue("height", "Sprites", 0);
  m_xOffset = a_iniDictionary.getIntValue("xOffset", "Sprites", 0);
  m_yOffset = a_iniDictionary.getIntValue("yOffset", "Sprites", 0);
  for (int iBitmapArea = 0; iBitmapArea < nBitmapAreas; iBitmapArea++) {
    char indexString[3];
    std::sprintf(indexString, "%d", iBitmapArea);
    std::string key("sprite");
    key += indexString;
    key += "YPos";
    m_pBitmapAreas[iBitmapArea].x = 0;
    m_pBitmapAreas[iBitmapArea].y = a_iniDictionary.getIntValue(key.c_str(), "Sprites", 0);
    m_pBitmapAreas[iBitmapArea].w = width;
    m_pBitmapAreas[iBitmapArea].h = height;
  }
  m_nAnimations = a_iniDictionary.getIntValue("amount", "Animations", 0);
  m_pAnimations = new Animation[m_nAnimations];
  for (int iAnimation = 0; iAnimation < m_nAnimations; iAnimation++) {
    char iAnimationString[3];
    std::sprintf(iAnimationString, "%d", iAnimation);
    std::string sectionName("Animation");
    sectionName += iAnimationString;
    int nFrames = a_iniDictionary.getIntValue("nFrames", sectionName.c_str(), 0);
    m_pAnimations[iAnimation].nFrames = nFrames;
    m_pAnimations[iAnimation].pFrames = new AnimationFrame[nFrames];
    for (int iFrame = 0; iFrame < nFrames; iFrame++) {
      char iFrameString[3];
      std::sprintf(iFrameString, "%d", iFrame);
      std::string spriteIndexKey("frame");
      spriteIndexKey += iFrameString;
      std::string durationKey(spriteIndexKey);
      spriteIndexKey += "SpriteIndex";
      m_pAnimations[iAnimation].pFrames[iFrame].index = a_iniDictionary.getIntValue(spriteIndexKey.c_str(), sectionName.c_str(), 0);
      durationKey += "Duration";
      m_pAnimations[iAnimation].pFrames[iFrame].duration = a_iniDictionary.getIntValue(durationKey.c_str(), sectionName.c_str(), 0);
    }
  }
}

Sprite::~Sprite() {
  for (int iAnimation = 0; iAnimation < m_nAnimations; iAnimation++) {
    delete [] m_pAnimations[iAnimation].pFrames;
  }
  delete [] m_pAnimations;
  delete [] m_pBitmapAreas;
  VideoService& videoService = ServiceLocator::getVideoService();
  for (int iBitmap = 0; iBitmap < m_nBitmaps; iBitmap++) {
    videoService.unloadBitmap(m_pBitmaps[iBitmap]);
  }
  delete [] m_pBitmaps;
}

void Sprite::animateForward() {
  if (m_duration == m_pAnimations[m_iCurrentAnimation].pFrames[m_iCurrentFrame].duration) {
    m_iCurrentFrame++;
    m_duration = 0;
    if (m_iCurrentFrame == m_pAnimations[m_iCurrentAnimation].nFrames) {
      m_iCurrentFrame = 0;
    }
  }
  m_duration++;
}

void Sprite::animateForwardBackward() {
  if (m_duration == m_pAnimations[m_iCurrentAnimation].pFrames[m_iCurrentFrame].duration) {
    m_duration = 0;
    int iLastFrame = m_pAnimations[m_iCurrentAnimation].nFrames - 1;
    if (m_step == 1 && m_iCurrentFrame == iLastFrame) {
      m_step = -1;
    }
    else if (m_step == -1 && m_iCurrentFrame == 0) {
      m_step = 1;
    }
    else {
      m_iCurrentFrame += m_step;
    }
  }
  m_duration++;
}

void Sprite::draw(int a_x, int a_y, int a_iBitmap) {
  a_x += m_xOffset;
  a_y += m_yOffset;
  int iBitmapArea = m_pAnimations[m_iCurrentAnimation].pFrames[m_iCurrentFrame].index;
  VideoService& videoService = ServiceLocator::getVideoService();
  videoService.copyToScreen(m_pBitmaps[a_iBitmap], a_x, a_y, &m_pBitmapAreas[iBitmapArea]);
}

void Sprite::resetCurrentAnimationFrameDuration() {
  m_duration = 0;
}

void Sprite::setCurrentAnimationIndex(int a_iAnimation) {
  if (a_iAnimation != m_iCurrentAnimation) {
    m_iCurrentAnimation = a_iAnimation;
    m_currentAnimationOrderHandler = &Sprite::animateForward;
    m_iCurrentFrame = 0;
    m_duration = 0;
  }
}

void Sprite::setForwardBackwardAnimationOrder() {
  m_currentAnimationOrderHandler = &Sprite::animateForwardBackward;
}

void Sprite::update() {
  (this->*m_currentAnimationOrderHandler)();
}
