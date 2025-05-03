#ifndef VIDEOSERVICE_H
#define VIDEOSERVICE_H

struct SDL_Rect;
struct SDL_Surface;

class VideoService {
  public:
  virtual ~VideoService() {};
  virtual void blitToScreen(SDL_Surface* a_pSourceBitmap, int a_x, int a_y, SDL_Rect* a_pSourceBitmapArea = 0) = 0;
  virtual void blitToSurface(SDL_Surface* a_pSourceBitmap, SDL_Surface* a_pDestinationBitmap, int a_destinationX, int a_destinationY, SDL_Rect* a_pSourceBitmapArea = 0) = 0;
  virtual void blitUiToScreen(SDL_Surface* a_pSourceBitmap, int a_destinationX, int a_destinationY, SDL_Rect* a_pSourceBitmapArea = 0) = 0;
  virtual void clearScreen() = 0;
  virtual SDL_Surface* createSurface(int a_width, int a_height) = 0;
  virtual SDL_Surface* loadSurface(const char* a_pFilename) = 0;
  virtual void unloadSurface(SDL_Surface* a_pSurface) = 0;
};

#endif
