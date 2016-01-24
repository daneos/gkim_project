#ifndef BMPTOOL_H_INCLUDED
#define BMPTOOL_H_INCLUDED

#include <sys/types.h>
#include <SDL/SDL.h>
#include "guitool.h"

typedef struct _conv_bmp {
	int height;
	int width;
	Uint8 bitsperpixel;
	Uint8 **red_color;
	Uint8 **green_color;
	Uint8 **blue_color;
} conv_bmp;

bool LoadBMP(char* filepath, conv_bmp* new_bmp);
void freeStruct(conv_bmp* new_bmp);

#endif // BMPTOOL_H_INCLUDED
