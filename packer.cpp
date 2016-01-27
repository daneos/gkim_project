/**
 * @package Byte-packing encoder/decoder
 * @file Implementation file
 * Projekt GKIM
 * Grzegorz Kowalski
 * Bartosz Zielnik
 * Piotr Mańkowski
 * Dariusz Szyszlak
 * version 1 | 01.2016
 */

#include "packer.h"

/**
 * Convert conv_bmp into byte-packed array
 * @param new_bmp structure which holds BMP essentials
 * @param size pointer to variable to hold the size of packed data
 * @return packed array
 */
uint8_t *pack(conv_bmp* new_bmp, int *size)
{
	uint8_t *out = (uint8_t*)malloc(new_bmp->height*new_bmp->width*3);
	Uint8 doublecolor;
	int i, j, z=0;
	for(i=0;i<new_bmp->height;i++)		//write pixel colors in order RGB
	{
		for(j=0;j<new_bmp->width;j++)
		{
			doublecolor = new_bmp->red_color[i][j] << 4;
			doublecolor = doublecolor | new_bmp->green_color[i][j];
			out[z++] = doublecolor;
			doublecolor = new_bmp->blue_color[i][j] << (Uint8) 4;
			doublecolor = doublecolor | new_bmp->red_color[i][j+1];
			out[z++] = doublecolor;
			doublecolor = new_bmp->green_color[i][j+1] << 4;
			doublecolor = doublecolor | new_bmp->blue_color[i][j+1];
			out[z++] = doublecolor;
			j++;
		}
	}
	*size = z;	// size of packed data
	return out;
}

/**
 * Unpack byte-packed array into the conv_bmp structure
 * @param width width of the image
 * @param height height of the image
 * @param data packed data
 * @return unpacked conv_bmp structure
 */
conv_bmp *unpack(int width, int height, uint8_t *data)
{
	Uint8 doublecolor;
	conv_bmp *new_bmp = (conv_bmp*)malloc(sizeof(conv_bmp));

	new_bmp->width = width;
	new_bmp->height = height;

	new_bmp->red_color = (Uint8**)malloc(sizeof(Uint8*) * height);  //allocate memory for colors
	new_bmp->green_color = (Uint8**)malloc(sizeof(Uint8*) * height);
	new_bmp->blue_color = (Uint8**)malloc(sizeof(Uint8*) * height);
	for(int i=0;i<height;i++)
	{
		new_bmp->red_color[i] = (Uint8*)malloc(sizeof(Uint8) * width);
		new_bmp->green_color[i] = (Uint8*)malloc(sizeof(Uint8) * width);
		new_bmp->blue_color[i] = (Uint8*)malloc(sizeof(Uint8) * width);
		for(int j=0; j<width;j++)
		{
			doublecolor = *(data++);
			new_bmp->red_color[i][j] = doublecolor >> 4;
			new_bmp->green_color[i][j] = doublecolor & 15;
			doublecolor = *(data++);
			new_bmp->blue_color[i][j] = doublecolor >> 4;
			new_bmp->red_color[i][j+1] = doublecolor & 15;
			doublecolor = *(data++);
			new_bmp->green_color[i][j+1] = doublecolor >> 4;
			new_bmp->blue_color[i][j+1] = doublecolor & 15;
			j++;
		}
	}
	return new_bmp;
}