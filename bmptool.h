#ifndef BMPTOOL_H_INCLUDED
#define BMPTOOL_H_INCLUDED

#include<sys/types.h>
#include<SDL/SDL.h>

struct conv_bmp {
	int height;
	int width;
	Uint8 bitsperpixel;
	Uint8 **red_color;
	Uint8 **green_color;
	Uint8 **blue_color;
};

#endif // BMPTOOL_H_INCLUDED
