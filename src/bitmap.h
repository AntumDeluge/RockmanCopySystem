#ifndef BITMAP_H
#define BITMAP_H

#  ifndef SDL2
struct SDL_Surface;
#  else
struct SDL_Texture;
#  endif

struct Bitmap {
#  ifndef SDL2
  SDL_Surface* pInternalObject;
#  else
  SDL_Texture* pInternalObject;
#  endif
  unsigned short width;
  unsigned short height;
};

#endif
