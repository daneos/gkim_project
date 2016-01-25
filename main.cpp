#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include "bmptool.h"
#include "surfacetool.h"
#include "packer.h"
#include "lz77.h"
#include "huffman.h"

#include <SDL/SDL.h>

#define ENC_NONE		0
#define ENC_HUFFMAN		1
#define ENC_LZ77		2

void help(char * pname)
{
	printf("\nhciconv - Hopefully Compressed Image converter\n");
	printf("(C) 2015-2016 Grzegorz Kowalski, Bartosz Zielnik, Piotr Mańkowski, Dariusz Szyszlak\n\n");
	printf("USAGE:\n");
	printf("\t%s <compression> [input file] [output file]\n", pname);
	printf("\tIf input file is .hci, then there are no options available,\n");
	printf("\tand the action is decoding.\n");
	printf("\tIf input file is .bmp, then following options are available,\n");
	printf("\tand the action is encoding:\n");
	printf("\t\tp\tbyte packing (done also when using other algorithms)\n");
	printf("\t\th\tHuffman compression\n");
	printf("\t\tl\tLZ77 compression\n");
	printf("\tProgram always cuts images to be 4-bit per channel.\n");
	printf("\tYou can add \'g\' to the compression type to convert the image to grayscale.\n\n");
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
		FILE *fin = fopen(argv[1], "rb");
		uint8_t id[3];
		
		fread(id, sizeof(uint8_t), 3, fin);		// read id
		
		if(strncmp((char*)id, "HCI", 3))
		{
			printf("Error! Input is not a HCI file. Exiting...\n");
			return -1;
		}

		int width;
		int height;
		fread(&width, sizeof(uint32_t), 1, fin);	// read image size
		fread(&height, sizeof(uint32_t), 1, fin);
		printf("Loaded image: HCI %dx%d\n", width, height);

		uint8_t encoding;
		
		fread(&encoding, sizeof(uint8_t), 1, fin);	// read encoding

		conv_bmp *bmp;		// here a decoded bmp image will be saved

		switch(encoding)
		{
			case ENC_NONE:
			{
				printf("Image is compressed using byte-packing algorithm.\n");
				// byte packing
				printf("=== Byte-packing started. ===\n");
				int bp_size;
				fread(&bp_size, sizeof(uint32_t), 1, fin);		// read size
				printf("Data size read. %d bytes.\n", bp_size);

				uint8_t *data = (uint8_t*)malloc(bp_size*sizeof(uint8_t));
				printf("Buffers allocated.\n");

				fread(data, sizeof(uint8_t), bp_size, fin);		// read data
				printf("Data read.\n");

				bmp = unpack(width, height, data);
				printf("Unpacking done.\n=== DONE ===\n");

				break;
			}

			case ENC_HUFFMAN:
			{
				printf("Image is compressed using Huffman algorithm.\n");
				// huffman
				printf("=== Huffman started. ===\n");
				Uint16 dictionary_size=0;
   				Uint16 the_longest_code=0;
   				Uint8 code_huff=0;
   				Uint16 code_length=0;
   				int csize;
   				fread(&dictionary_size, sizeof(Uint16), 1, fin);
   				fread(&the_longest_code, sizeof(Uint16), 1, fin);
   				printf("Parameters read.\n");
   				printf("%d dict size \n",dictionary_size );
   				printf("%d longest code\n", the_longest_code);

   				dictionary *elements_of_dictionary=new dictionary[dictionary_size];
   				printf("Dictionary allocated.\n");
   				
   				for(int i=0; i<dictionary_size; i++)
                {
                 elements_of_dictionary[i].key=i;
                   	 fread(&elements_of_dictionary[i].colors.r,sizeof(Uint8),1,fin);
                 	 fread(&elements_of_dictionary[i].colors.g,sizeof(Uint8),1,fin);
      				 fread(&elements_of_dictionary[i].colors.b,sizeof(Uint8),1,fin);
       			     fread(&code_length,sizeof(Uint8),1,fin);
      				 elements_of_dictionary[i].code_length=code_length;

        			for(int j=0; j<elements_of_dictionary[i].code_length; j++)
        			{
           				  fread(&code_huff,sizeof(Uint8),1,fin);
            				 elements_of_dictionary[i].huffmancode[j]=code_huff;
            				 code_huff=0;
       				 }
   			   }
   			   printf("Dictionary read.\n");

   				fread(&csize, sizeof(int),1,fin);
   				printf("%d csize\n",csize );
   				Uint8 *in = new Uint8[csize];
   				printf("Buffers allocated.\n");
   				fread(in,sizeof(Uint8),csize,fin);
   				printf("Data read.\n");
				bmp = huffman_decoding(height,width,dictionary_size,the_longest_code,elements_of_dictionary,in,csize);
				printf("Decompression done.\n=== DONE ===\n");
				break;
			}

			case ENC_LZ77:
			{
				printf("Image is compressed using LZ77 algorithm.\n");
				// lz77
				printf("=== LZ77 started. ===\n");
				uint8_t lz_dict_size;
				uint32_t lz_size;
				fread(&lz_dict_size, sizeof(uint8_t), 1, fin);	// read lz77 parameters
				fread(&lz_size, sizeof(uint32_t), 1, fin);
				printf("Parameters read.\n");
				printf("dictionary = %d\nlz_size = %d\n", lz_dict_size, lz_size);
				
				long data_start = ftell(fin);
				fseek(fin, 0, SEEK_END);
				long data_end = ftell(fin);
				long data_len = data_end - data_start;			// find out length of data
				printf("Data size read. %ld bytes.\n", data_len);
				
				fseek(fin, data_start, SEEK_SET);				// return to previous position

				uint8_t *data = (uint8_t*)malloc(data_len*sizeof(uint8_t));
				fread(data, sizeof(uint8_t), data_len, fin);	// read compressed data
				printf("Data read.\n");

				uint8_t *out = (uint8_t*)malloc(lz_size*sizeof(uint8_t));
				printf("Buffers allocated.\n");

				lz77_decompress(data, out, data_len, lz_size, lz_dict_size);
				free(data);
				printf("Decompression done.\n");

				bmp = unpack(width, height, out);
				//free(out);
				printf("Unpacking done.\n=== DONE ===\n");

				break;
			}

			default:
			{
				printf("Error! Compression code %d not known. Input file is probably corrupted. Exiting...\n", encoding);
				return -1;
			}
		}

		SDL_Surface *sur = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		printf("New surface created.\n");
		
		// push image into surface
		for(int i=0; i < height; i++)
			for(int j=0; j < width; j++)
				setPixel(j, i, sur, bmp->red_color[i][j]*16, bmp->green_color[i][j]*16, bmp->blue_color[i][j]*16);
		printf("Image pushed.\n");

		SDL_SaveBMP(sur, argv[2]);		// save surface to bmp file
		printf("Saved.\n=== DONE ===\n");

		// clean
		freeStruct(bmp);
		free(bmp);
		SDL_FreeSurface(sur);
	}
	else
	{
		// coding
		conv_bmp bmp;
		LoadBMP(argv[2], &bmp);
		uint32_t height = bmp.height;
		uint32_t width = bmp.width;
		printf("Loaded image: BMP %dx%d\n", width, height);
		uint8_t id[3] = { 'H', 'C', 'I' };
		uint8_t encoding = ENC_NONE;

		if(strlen(argv[1]) > 1)
			if(argv[1][1] == 'g' || argv[1][1] == 'G')	// if first argument is e.g. "hg" use grayscale
			{
				printf("Using grayscale.\n");
				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < width; j++)
					{
						// convert image to grayscale
						uint8_t avg = floor((bmp.red_color[i][j]*16 + bmp.green_color[i][j]*16 + bmp.blue_color[i][j]*16)/3);
						bmp.red_color[i][j] = avg/16;
						bmp.green_color[i][j] = avg/16;
						bmp.blue_color[i][j] = avg/16;
					}
				}
				printf("Image converted to grayscale.\n");
			}

		uint8_t *out;	// ouput data
		uint32_t csize;		// size of compressed data

		// byte packing specific data
		uint32_t bp_size;

		// huffman specific data
		Uint16 	dictionary_size=0;
		Uint16  longest_code=0;
		Uint8 length=0;
		Uint8 huffman_code;
		dictionary* elements_of_dictionary=new dictionary[4096];

		// lz77 specific data
		uint8_t lz_dict_size;	//	dictionary size
		uint32_t lz_size;		// uncompressed data size

		switch(argv[1][0])
		{
			case 'p':
			case 'P':
			{
				// byte packing
				encoding = ENC_NONE;
				printf("=== Byte-packing started. ===\n");
				out = pack(&bmp, (int*)&bp_size);
				freeStruct(&bmp);
				printf("Packed succesfully. %d bytes.\n=== DONE ===\n", bp_size);
				break;
			}

			case 'h':
			case 'H':
			{
				// huffman
				encoding = ENC_HUFFMAN;
				printf("=== Huffman started. ===\n");

				out = (uint8_t*)malloc(bmp.width*bmp.height*sizeof(uint8_t));
				printf("Buffers allocated.\n");

				csize = huffman_encoding(&bmp,out,dictionary_size,longest_code,elements_of_dictionary);
				printf("Compression done. %d bytes.\n=== DONE ===\n", csize);
			
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
				printf("dictionary = %d\nlookahead = %d\n", lz_dict_size, lookahead_size);

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
			printf("Main header written.\n");
			// write header for compression algorithm
			if(encoding == ENC_NONE)
			{
				fwrite(&bp_size, sizeof(uint32_t), 1, fout);	// bp compressed data size
				printf("ENC_NONE header written.\n");
			}
			else if(encoding == ENC_HUFFMAN)
			{
				// Huffman header
				fwrite(&dictionary_size, sizeof(Uint16), 1, fout);	// width
				fwrite(&longest_code, sizeof(Uint16), 1, fout);	// width
				printf("dictionary_size  %d\n",dictionary_size);
				for(int i=0; i<dictionary_size; i++)
    				{
				        fwrite(&elements_of_dictionary[i].colors.r, sizeof(Uint8) ,1, fout);
				        fwrite(&elements_of_dictionary[i].colors.g, sizeof(Uint8) ,1, fout);
				        fwrite(&elements_of_dictionary[i].colors.b, sizeof(Uint8) ,1, fout);
				        length=elements_of_dictionary[i].code_length|length;
				        fwrite(&length,sizeof(Uint8),1,fout);
				        length=0;

	      			 for(int j=0; j<elements_of_dictionary[i].code_length; j++)
				        {
				           // huffman_code<<1;
				            huffman_code=elements_of_dictionary[i].huffmancode[j];
				            fwrite(&huffman_code,sizeof(huffman_code),1,fout);
				            huffman_code=0;
				        }
   					 }
   			    fwrite(&csize, sizeof(uint32_t), 1, fout);
				printf("ENC_HUFFMAN header written.\n");
			}
			else if(encoding == ENC_LZ77)
			{
				fwrite(&lz_dict_size, sizeof(uint8_t), 1, fout);	// lz dictionary size
				fwrite(&lz_size, sizeof(uint32_t), 1, fout);		// lz uncompressed size
				printf("ENC_LZ77 header written.\n");
			}
			// write compressed data
			fwrite(out, sizeof(uint8_t), csize, fout);
			printf("Data written.\n");
			free(out);
			fclose(fout);
			printf("Saved.\n=== DONE ===\n");
		}
		else
		{
			printf("Error opening file %s!\n", argv[3]);
			return -1;
		}
	}

	return 0;
}

