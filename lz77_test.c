#include <stdio.h>
#include <stdlib.h>
#include "lz77.h"

int printbuf(uint8_t *buf, int len)
{
	for(int i=0; i<len; i++)
		printf("%d, ", buf[i]);
	printf("\n");
}

int main(void)
{
	uint8_t in[24] = {1,2,3,3,1,1,2,3,4,1,2,4,4,1,3,1,4,2,1,2,4,3,1,3};//{1,4,4,1,4,4,1,4,4,1,4,4,1,4,4,1,4,4,1,4,4,1,4,4};
	uint8_t out[100];
	uint8_t dec[24];

	// for(int i=0; i < 1000; i++)
	// 	in[i] = rand()%256;

	int clen = lz77_compress(in, out, 24, 16, 4);
	printf("-------------------------------------------------------------------------------------\n");
	lz77_decompress(out, dec, clen, 24, 16);

	printbuf(in, 24);
	printf("\n");
	printbuf(out, clen);
	printf("Size: %d\n\n", clen);
	
	printbuf(in, 24);
	printbuf(dec, 24);
}