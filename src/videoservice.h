#ifndef VIDEOSERVICE_H
#define VIDEOSERVICE_H

#include "bitmap.h"

struct Rect;

class VideoService {
  public:
  virtual ~VideoService() {};
  virtual void clearScreen() = 0;
  virtual void copyToBitmap(Bitmap& a_sourceBitmap, Bitmap& a_destinationBitmap, int a_destinationX, int a_destinationY, Rect* a_pSourceBitmapArea = 0) = 0;
  virtual void copyToScreen(Bitmap& a_sourceBitmap, int a_x, int a_y, Rect* a_pSourceBitmapArea = 0) = 0;
  virtual void createBitmap(Bitmap& a_bitmap, int a_width, int a_height) = 0;
  virtual void loadBitmap(Bitmap& a_bitmap, const char* a_pFilename) = 0;
  virtual void unloadBitmap(Bitmap& a_bitmap) = 0;
};

#endif
