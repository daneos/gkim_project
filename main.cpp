// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
//#include<cstdint>
#include "huffman.h"
#include"bmptool.h"

#include <SDL/SDL.h>


/**
 * Set pixel color in SDL_Surface
 * @param x x axis position
 * @param y y axis position
 * @param screen SDL_Surface
 * @param R amount of red color
 * @param G amount of green color
 * @param B amount of blue color
 */
//extern huffman_decoding(conv_bmp* new_bmp);
void setPixel(int x, int y,SDL_Surface* screen, Uint8 R, Uint8 G, Uint8 B)
{
  if ((screen!=NULL) && (x>=0) && (x<screen->w) && (y>=0) && (y<screen->h))
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
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
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
SDL_Color getPixel(int x, int y, SDL_Surface* screen) {
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

/**
 * Load BMP file to conv_bmp structure
 * @param filepath BMP file path
 * @param new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */
bool LoadBMP(char* filepath, conv_bmp* new_bmp)  {
    SDL_Surface* bmp = SDL_LoadBMP(filepath);
    if(bmp < 0) {
        printf("SDL_LoadBMP failed: %s\n", SDL_GetError());
        return false;
    }
    else {
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
 * Save conv_bmp structure into binary file
 * @param filepath path for binary file
 * @param new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */
bool SaveToBinary(char* filepath, conv_bmp* new_bmp) {
    FILE* pFile = fopen(filepath, "wb");  //wb = write and binary
    if(pFile == NULL) {
        std::cout << "Cannot open binary file " << filepath << std::endl;
        return false;
    } else {
        Uint8 doublecolor;
        fwrite(&new_bmp->height,sizeof(int),1, pFile);  //write header
        fwrite(&new_bmp->width,sizeof(int),1, pFile);
        fwrite(&new_bmp->bitsperpixel,sizeof(Uint8),1, pFile);
        for(int i=0;i<new_bmp->height;i++) {            //write pixel colors in order RGB
            for(int j=0;j<new_bmp->width;j++) {
                doublecolor = new_bmp->red_color[i][j] << 4;
                doublecolor = doublecolor | new_bmp->green_color[i][j];
                fwrite(&doublecolor,sizeof(Uint8),1,pFile);
                doublecolor = new_bmp->blue_color[i][j] << (Uint8) 4;
                doublecolor = doublecolor | new_bmp->red_color[i][j+1];
                fwrite(&doublecolor,sizeof(Uint8),1,pFile);
                doublecolor = new_bmp->green_color[i][j+1] << 4;
                doublecolor = doublecolor | new_bmp->blue_color[i][j+1];
                fwrite(&doublecolor,sizeof(Uint8),1,pFile);
                j++;
            }
        }
        return true;
    }
    fclose(pFile);
}

/**
 * Load binary file to conv_bmp structure
 * @param filepath path for binary file
 * @param new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */

bool LoadFromBinary(char* filepath, conv_bmp* new_bmp) {
    FILE* pFile = fopen(filepath, "rb");  //rb = read and binary
    if(pFile == NULL) {
        std::cout << "Cannot open binary file " << filepath << std::endl;
        return false;
    } else {
        Uint8 doublecolor;
        fread(&new_bmp->height,sizeof(int),1, pFile);  //read header
        fread(&new_bmp->width,sizeof(int),1, pFile);
        fread(&new_bmp->bitsperpixel,sizeof(Uint8),1, pFile);
        new_bmp->red_color = (Uint8**)malloc(sizeof(Uint8*) * new_bmp->height);  //allocate memory for colors
        new_bmp->green_color = (Uint8**)malloc(sizeof(Uint8*) * new_bmp->height);
        new_bmp->blue_color = (Uint8**)malloc(sizeof(Uint8*) * new_bmp->height);
        for(int i=0;i<new_bmp->height;i++) {
            new_bmp->red_color[i] = (Uint8*)malloc(sizeof(Uint8) * new_bmp->width);
            new_bmp->green_color[i] = (Uint8*)malloc(sizeof(Uint8) * new_bmp->width);
            new_bmp->blue_color[i] = (Uint8*)malloc(sizeof(Uint8) * new_bmp->width);
            for(int j=0; j<new_bmp->width;j++) {
                fread(&doublecolor,sizeof(Uint8),1,pFile);
                new_bmp->red_color[i][j] = doublecolor >> 4;
                new_bmp->green_color[i][j] = doublecolor & 15;
                fread(&doublecolor,sizeof(Uint8),1,pFile);
                new_bmp->blue_color[i][j] = doublecolor >> 4;
                new_bmp->red_color[i][j+1] = doublecolor & 15;
                fread(&doublecolor,sizeof(Uint8),1,pFile);
                new_bmp->green_color[i][j+1] = doublecolor >> 4;
                new_bmp->blue_color[i][j+1] = doublecolor & 15;
                j++;
            }
        }
    }
    fclose(pFile);
    return true;
}

/**
 * Free memory used by conv_bmp structure
 * @param new_bmp structure which holds BMP essentials
 */
void freeStruct(conv_bmp* new_bmp) {
    for (int i = 0; i < new_bmp->height; i++)
        {
            free(new_bmp->red_color[i]);
            free(new_bmp->green_color[i]);
            free(new_bmp->blue_color[i]);
        }

        free(new_bmp->red_color);
        free(new_bmp->green_color);
        free(new_bmp->blue_color);
}

int main( int argc, char** argv )
{
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );
    SDL_Surface* test = NULL;

    conv_bmp new_bmp;

    LoadBMP("images.bmp",&new_bmp);
   // SaveToBinary("file.binary",&new_bmp);
   // LoadFromBinary("file.binary",&new_bmp);
   // huffman_encoding(&new_bmp);
   // huffman_decoding(&new_bmp);

     if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            printf( "Unable to init SDL: %s\n", SDL_GetError() );
            return 1;
        }
        atexit(SDL_Quit);

        // create a new window
        test = SDL_SetVideoMode(new_bmp.width, new_bmp.height, 32,
                   SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT);
        if ( !test )
        {
            printf("Unable to set video: %s\n", SDL_GetError());
            return 1;
        }
        bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    if (event.key.keysym.sym == SDLK_c) {    //4 biotwy obraz
                        for(int i=0;i<new_bmp.height;i++) {
                            for(int j=0;j<new_bmp.width;j++) {
                                setPixel(j,i,test,new_bmp.red_color[i][j]*16,new_bmp.green_color[i][j]*16,new_bmp.blue_color[i][j]*16);
                            }
                        }
                        SDL_Flip(test);
                     }
                     if (event.key.keysym.sym == SDLK_g) {   //4 bitowa skala szarosci
                        for(int i=0;i<new_bmp.height;i++) {
                            for(int j=0;j<new_bmp.width;j++) {
                                Uint8 grey = (new_bmp.red_color[i][j]*16 + new_bmp.green_color[i][j]*16 + new_bmp.blue_color[i][j]*16) /3;
                                setPixel(j,i,test,grey,grey,grey);
                            }
                        }
                        SDL_Flip(test);
                     }
                    if (event.key.keysym.sym == SDLK_1) {   //4 bitowa skala szarosci
                        LoadBMP("images.bmp",&new_bmp);
                        SaveToBinary("file.binary",&new_bmp);
                     }
                    if (event.key.keysym.sym == SDLK_2) {   //4 bitowa skala szarosci
                        LoadFromBinary("file.binary",&new_bmp);
                        SDL_SaveBMP(test,"new.bmp");
                     }
                    if (event.key.keysym.sym == SDLK_h) {   //kodowanie Huffmana
                        huffman_encoding(&new_bmp);
                     }
                    if (event.key.keysym.sym == SDLK_j) {   //dekodowanie  Huffmana
                        huffman_decoding(&new_bmp);
                        SDL_SaveBMP(test,"new_huff.bmp");
                     }
                }
            } // end switch
        } // end of message processing

    } // end main loop
freeStruct(&new_bmp);

	if(test) SDL_FreeSurface(test);
	printf("Exited cleanly\n");
	return 0;
}

