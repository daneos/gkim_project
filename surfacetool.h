#if !defined(__SURFACETOOL_H__)
#	define __SURFACETOOL_H___

#include <SDL/SDL.h>

void setPixel(int x, int y,SDL_Surface* screen, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y, SDL_Surface* screen);

#endif /* __GUITOOL_H___ */