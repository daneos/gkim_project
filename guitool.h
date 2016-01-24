#if !defined(__GUITOOL_H__)
#	define __GUITOOL_H___

#include <SDL/SDL.h>

void setPixel(int x, int y,SDL_Surface* screen, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y, SDL_Surface* screen);

#endif /* __GUITOOL_H___ */