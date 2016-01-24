#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "bmptool.h"
#include "guitool.h"
#include "packer.h"
#include "lz77.h"
#include "huffman.h"

#include <SDL/SDL.h>

#define ENC_NONE		0
#define ENC_HUFFMAN		1
#define ENC_LZ77		2


void help(char * pname)
{
	printf("hciconv - Hopefully Compressed Image converter\n");
	printf("(C) 2015-2016 Grzegorz Kowalski, Bartosz Zielnik, Piotr Mańkowski, Dariusz Szyszlak\n\n");
	printf("USAGE:\n");
	printf("\t%s [options] [input file] [output file]\n", pname);
	printf("\tIf input file is .hci, then there are no options available,\n");
	printf("\tand the action is decoding.\n");
	printf("\tIf input file is .bmp, then following options are available,\n");
	printf("\tand the action is encoding:\n");
	printf("\t\t h\tHuffman compression\n");
	printf("\t\t l\tLZ77 compression\n");
	printf("\tProgram always cuts images to be 4-bit per channel.\n\n");
}

int main(int argc, char** argv)
{

	if(argc < 3)
	{
		help(argv[0]);
		return 0;
	}
	if(argc < 4)
	{
		// decoding
	}
	else
	{
		// coding
		conv_bmp bmp;
		LoadBMP(argv[2], &bmp);
		uint32_t height = bmp.height;
		uint32_t width = bmp.width;
		uint8_t id[3] = { 'H', 'C', 'I' };
		uint8_t encoding = ENC_NONE;

		uint8_t *out;	// ouput data
		int csize;		// size of compressed data

		// tutaj wrzuccie zmienne potrzebne do zapisu
		uint8_t lz_dict_size;	//	dictionary size
		uint32_t lz_size;		// uncompressed data size

		switch(argv[1][0])
		{
			case 'h':
			case 'H':
			{
				// huffman
				encoding = ENC_HUFFMAN;
				break;
			}
		
			case 'l':
			case 'L':
			{
				// lz77
				encoding = ENC_LZ77;
				lz_dict_size = 255;
				int lookahead_size = 8;
				printf("=== LZ77 started. ===\n");

				uint8_t *packed = pack(&bmp, (int*)&lz_size);
				freeStruct(&bmp);
				printf("Packed succesfully. %d bytes.\n", lz_size);

				uint8_t *prepared = prepare(packed, lz_size, lz_dict_size, lookahead_size);
				free(packed);
				out = (uint8_t*)malloc(lz_size*sizeof(uint8_t)*2);	// lz77 compressed data can be slightly bigger than uncompressed
				printf("Buffers allocated.\n");

				csize = lz77_compress(prepared, out, lz_size, lz_dict_size, lookahead_size);
				//free(prepared);
				printf("Compression done. %d bytes.\n=== DONE ===\n", csize);

				break;
			}
		
			default:
			{
				printf("Unrecognized option: %s\n\n", argv[1]);
				help(argv[0]);
				return -1;
			}
		}

		FILE *fout = fopen(argv[3], "wb");
		if(fout)
		{
			// write header
			fwrite(id, sizeof(uint8_t), 3, fout);		// file id
			fwrite(&width, sizeof(uint32_t), 1, fout);	// width
			fwrite(&height, sizeof(uint32_t), 1, fout);	// height
			fwrite(&encoding, sizeof(uint8_t), 1, fout);// encoding type
			// write header for compression algorithm
			if(encoding == ENC_HUFFMAN)
			{
				// tutaj zapis naglowka dla huffmana
			}
			else if(encoding == ENC_LZ77)
			{
				fwrite(&lz_dict_size, sizeof(uint8_t), 1, fout);	// lz dictionary size
				fwrite(&lz_size, sizeof(uint32_t), 1, fout);		// lz uncompressed size
			}
			// write compressed data
			fwrite(out, sizeof(uint8_t), csize, fout);
			free(out);
			fclose(fout);
		}
		else
		{
			printf("Error opening file %s!\n", argv[3]);
			return -1;
		}
	}

	return 0;
}

