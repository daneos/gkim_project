// ConsoleApplication1.cpp : Defines the entry point for the console application.
//


#include <stdlib.h>

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include<iostream>

#include <SDL/SDL.h>

struct conv_bmp {
	int height;
	int width;
	Uint8 bitsperpixel;
	Uint8 **red_color;
	Uint8 **green_color;
	Uint8 **blue_color;
};

void setPixel(int x, int y,SDL_Surface* screen, Uint8 R, Uint8 G, Uint8 B)
{
  if ((screen!=NULL) && (x>=0) && (x<screen->w) && (y>=0) && (y<screen->h))
  {
    /* Zamieniamy poszczególne sk³adowe koloru na format koloru pixela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informacji ile bajtów zajmuje jeden pixel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres pixela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartoœæ pixela, w zale¿noœci od formatu powierzchni*/
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
         /* update the screen (aka double buffering) */
  }
}

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

bool LoadBMP(char* filepath, conv_bmp* new_bmp)  {
    SDL_Surface* bmp = SDL_LoadBMP(filepath);
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
			new_bmp->red_color[i][j] = color.r ;   //nie jestem pewien czy ja mam to skrocic do 4 bitow czy w przy algorytmach kompresji, spytam go w czwartek.
			new_bmp->green_color[i][j] = color.g ;
			new_bmp->blue_color[i][j] = color.b ;
			printf("Pixel Color -> R: %d,  G: %d,  B: %d, \n", new_bmp->red_color[i][j], new_bmp->green_color[i][j], new_bmp->blue_color[i][j]);
		}
	}
		if(bmp) SDL_FreeSurface(bmp);
}


int main( int argc, char** argv )
{
    SDL_Surface* test = NULL;
    conv_bmp new_bmp;
    LoadBMP("cb.bmp",&new_bmp);

	    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }
	atexit(SDL_Quit);

    // create a new window
    test = SDL_SetVideoMode(new_bmp.width, new_bmp.height, 8,
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
                    if (event.key.keysym.sym == SDLK_1) {
                            for(int i=0;i<new_bmp.width;i++) {
                                for(int j=0;j<new_bmp.height;j++) {
                                    setPixel(i,j,test,new_bmp.red_color[j][i],new_bmp.green_color[j][i],new_bmp.green_color[j][i]);
                                }
                            }
                            SDL_Flip(test);
                     }
                }
            } // end switch
        } // end of message processing

    } // end main loop

for (int i = 0; i < new_bmp.height; i++)
	{
		free(new_bmp.red_color[i]);
		free(new_bmp.green_color[i]);
		free(new_bmp.blue_color[i]);
	}

	free(new_bmp.red_color);
	free(new_bmp.green_color);
	free(new_bmp.blue_color);
	if(test) SDL_FreeSurface(test);
	printf("Exited cleanly\n");
	return 0;
}
