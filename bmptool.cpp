/**
 * @package BMP image tools
 * @file Implementation file
 * Projekt GKIM
 * Grzegorz Kowalski
 * Bartosz Zielnik
 * Piotr Mańkowski
 * Dariusz Szyszlak
 * version 1 | 01.2016
 */

#include "bmptool.h"

/**
 * Load BMP file to conv_bmp structure
 * @param filepath BMP file path
 * @param new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */
bool LoadBMP(char* filepath, conv_bmp* new_bmp)
{
	SDL_Surface* bmp = SDL_LoadBMP(filepath);
	if(bmp < 0)
	{
		printf("SDL_LoadBMP failed: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		memset(new_bmp,0,sizeof(conv_bmp));
		new_bmp->height = bmp->h;
		new_bmp->width = bmp->w;
		new_bmp->bitsperpixel = 4;
		new_bmp->red_color = (Uint8**)malloc(sizeof(Uint8*) * bmp->h);
		new_bmp->green_color = (Uint8**)malloc(sizeof(Uint8*) * bmp->h);
		new_bmp->blue_color = (Uint8**)malloc(sizeof(Uint8*) * bmp->h);

		for(int i=0;i<bmp->h;i++)
		{
			new_bmp->red_color[i] = (Uint8*)malloc(sizeof(Uint8) * bmp->w);
			new_bmp->green_color[i] = (Uint8*)malloc(sizeof(Uint8) * bmp->w);
			new_bmp->blue_color[i] = (Uint8*)malloc(sizeof(Uint8) * bmp->w);

			for(int j=0;j<bmp->w;j++)
			{
				SDL_Color color = getPixel(j, i, bmp);
				new_bmp->red_color[i][j] = (color.r/16);
				new_bmp->green_color[i][j] = (color.g/16);
				new_bmp->blue_color[i][j] = (color.b/16);
				//printf("Pixel Color -> R: %d,  G: %d,  B: %d, \n", new_bmp->red_color[i][j], new_bmp->green_color[i][j], new_bmp->blue_color[i][j]);
			}
		}
		SDL_FreeSurface(bmp);
		return true;
	}
}

/**
 * Free memory used by conv_bmp structure
 * @param new_bmp structure which holds BMP essentials
 */
void freeStruct(conv_bmp* new_bmp)
{
	for(int i = 0; i < new_bmp->height; i++)
	{
		free(new_bmp->red_color[i]);
		free(new_bmp->green_color[i]);
		free(new_bmp->blue_color[i]);
	}

	free(new_bmp->red_color);
	free(new_bmp->green_color);
	free(new_bmp->blue_color);
}
