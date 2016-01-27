/**
 * @package SDL surface tools
 * @file Implementation file
 * Projekt GKIM
 * Grzegorz Kowalski
 * Bartosz Zielnik
 * Piotr Mańkowski
 * Dariusz Szyszlak
 * version 1 | 01.2016
 */

#include "surfacetool.h"

/**
 * Set pixel color in SDL_Surface
 * @param x x axis position
 * @param y y axis position
 * @param screen SDL_Surface
 * @param R amount of red color
 * @param G amount of green color
 * @param B amount of blue color
 */
void setPixel(int x, int y,SDL_Surface* screen, Uint8 R, Uint8 G, Uint8 B)
{
	if((screen!=NULL) && (x>=0) && (x<screen->w) && (y>=0) && (y<screen->h))
	{
		Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);
		int bpp = screen->format->BytesPerPixel;
		Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
		switch(bpp)
		{
			case 1: //8-bit
				*p = pixel;
				break;

			case 2: //16-bit
				*(Uint16 *)p = pixel;
				break;

			case 3: //24-bit
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				{
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
				}
				else
				{
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
				}
				break;

			case 4: //32-bit
				*(Uint32 *)p = pixel;
				break;

		}
	}
}

/**
 * Get pixel color from SDL_Surface
 * @param x x axis position
 * @param y y axis position
 * @param screen SDL_Surface
 * @return pixel color
 */
SDL_Color getPixel(int x, int y, SDL_Surface* screen)
{
	SDL_Color color;
	Uint32 col = 0;
	//determine position
	char* pPosition = (char*)screen->pixels;
	//offset by y
	pPosition += (screen->pitch*y);
	//offset by x
	pPosition += (screen->format->BytesPerPixel*x);
	//copy pixel data
	memcpy(&col, pPosition, screen->format->BytesPerPixel);
	//convert color
	SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
	return (color);
}
