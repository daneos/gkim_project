#include "packer.h"

/**
 * Save conv_bmp structure into binary file
 * @param filepath path for binary file
 * @param new_bmp structure which holds BMP essentials
 * @return true if all done correctly
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
 * Load binary file to conv_bmp structure
 * @param filepath path for binary file
 * @param new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */
conv_bmp *unpack(int width, int height, uint8_t *data)
{
	Uint8 doublecolor;
	conv_bmp *new_bmp = (conv_bmp*)malloc(sizeof(conv_bmp));
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