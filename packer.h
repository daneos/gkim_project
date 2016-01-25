#if !defined(__PACKER_H__)
#	define __PACKER_H___

#include "bmptool.h"

uint8_t *pack(conv_bmp* bmp, int *size);
conv_bmp *unpack(int width, int height, uint8_t* data);

#endif /* __PACKER_H___ */