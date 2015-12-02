#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>

#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include <math.h>
#define pi 3.14
#include <time.h>
#include <iostream>
using namespace std;

SDL_Surface *screen;
int width = 900;
int height = 600;
char const* tytul = "GKiM - Lab 3 - Nazwisko Imie";


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);

void Linia(int x1, int y1, int x2, int y2, Uint8 R, Uint8 G, Uint8 B);
void Okrag(int x, int y, int r, Uint8 R, Uint8 G, Uint8 B);
void Elipsa(int x, int y, int a, int b, Uint8 R, Uint8 G, Uint8 B);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);


void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();

void Funkcja1() {



//...


    SDL_Flip(screen);

}


void Funkcja2() {



//...


    SDL_Flip(screen);

}


void Funkcja3() {



//...


    SDL_Flip(screen);

}


void Funkcja4() {



//...


    SDL_Flip(screen);

}


void Funkcja5() {



//...


    SDL_Flip(screen);

}



void Linia(int x1, int y1, int x2, int y2, Uint8 R, Uint8 G, Uint8 B) {
    setPixel(x1,y1,R,G,B);
    setPixel(x2,y2,R,G,B);
     // zmienne pomocnicze
     int d, dx, dy, ai, bi, xi, yi;
     int x = x1, y = y1;
     // ustalenie kierunku rysowania
     if (x1 < x2)
     {
         xi = 1;
         dx = x2 - x1;
     }
     else
     {
         xi = -1;
         dx = x1 - x2;
     }
     // ustalenie kierunku rysowania
     if (y1 < y2)
     {
         yi = 1;
         dy = y2 - y1;
     }
     else
     {
         yi = -1;
         dy = y1 - y2;
     }
     // pierwszy piksel
     setPixel(x, y, R, G, B);
     // oś wiodąca OX
     if (dx > dy)
     {
         ai = (dy - dx) * 2;
         bi = dy * 2;
         d = bi - dx;
         // pętla po kolejnych x
         while (x != x2)
         {
             // test współczynnika
             if (d >= 0)
             {
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 x += xi;
             }
             setPixel(x, y, R, G, B);
         }
     }
     // oś wiodąca OY
     else
     {
         ai = ( dx - dy ) * 2;
         bi = dx * 2;
         d = bi - dy;
         // pętla po kolejnych y
         while (y != y2)
         {
             // test współczynnika
             if (d >= 0)
             {
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 y += yi;
             }
             setPixel(x, y, R, G, B);
         }
     }
    SDL_Flip(screen);
}

void RysujOkrag(int x0, int y0, int x, int y, Uint8 R, Uint8 G, Uint8 B){
    setPixel(x+x0, y+y0, R, G, B);
    setPixel(y+x0, x+y0, R, G, B);
    setPixel(y+x0, -x+y0, R, G, B);
    setPixel(x+x0, -y+y0, R, G, B);
    setPixel(-x+x0, -y+y0, R, G, B);
    setPixel(-y+x0, -x+y0, R, G, B);
    setPixel(-y+x0, x+y0, R, G, B);
    setPixel(-x+x0, y+y0, R, G, B);;
}


void Okrag(int x0, int y0, int r, Uint8 R, Uint8 G, Uint8 B) {
    int x =0;
    int y=r;
    int d = 3-2*r;

    while (x <= y){
        if (d<0){
            d=d+4*x+6;
            RysujOkrag(x0, y0, x, y, R, G, B);
        } else{
            d=d+4*(x-y)+10;
            y--;
            RysujOkrag(x0, y0, x, y, R, G, B);
        }
        x++;
    }
    SDL_Flip(screen);
}

void Elipsa(int x, int y, int a, int b, Uint8 R, Uint8 G, Uint8 B) {
    int xc  = 0,
        yc  = b;
    int aa  = a * a,
        aa2 = aa + aa,
        bb  = b * b,
        bb2 = bb + bb;
    int d   = bb - aa * b + (aa / 4),
        dx  = 0,
        dy  = aa2 * b;

    while (dx < dy) {
        setPixel (x - xc,y - yc, R, G, B);
        setPixel (x - xc,y + yc, R, G, B);
        setPixel (x + xc,y - yc, R, G, B);
        setPixel (x + xc,y + yc, R, G, B);
        if (d > 0){
            yc  = yc-1;
            dy -= aa2;
            d  -= dy;
        }
        xc  =xc+1;
        dx += bb2;
        d  += bb + dx;
    }
    d += (3 * ((aa - bb) / 2) - (dx + dy) / 2);

    while (yc >= 0) {
        setPixel (x - xc,y - yc, R, G, B);
        setPixel (x - xc,y + yc, R, G, B);
        setPixel (x + xc,y - yc, R, G, B);
        setPixel (x + xc,y + yc, R, G, B);
        if (d < 0) {
            xc =xc+1;
            dx += bb2;
            d  += (bb + dx);
        }
        yc  = yc-1;
        dy -= aa2;
        d  += aa - dy;
    }
    SDL_Flip(screen);
}




void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<width) && (y>=0) && (y<height))
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


void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Rect dstrect;
        dstrect.x = x;
        dstrect.y = y;
        SDL_BlitSurface(bmp, 0, screen, &dstrect);
        SDL_Flip(screen);
        SDL_FreeSurface(bmp);
    }

}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_Flip(screen);

}




SDL_Color getPixel (int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<width) && (y>=0) && (y<height)) {
        //determine position
        char* pPosition=(char*)screen->pixels ;
        //offset by y
        pPosition+=(screen->pitch*y) ;
        //offset by x
        pPosition+=(screen->format->BytesPerPixel*x);
        //copy pixel data
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        //convert color
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}







int main ( int argc, char** argv )
{
    // console output
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(width, height, 32,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption( tytul , NULL );
    // program main loop
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
                    if (event.key.keysym.sym == SDLK_1)
                        Funkcja1();
                    if (event.key.keysym.sym == SDLK_2)
                        Funkcja2();
                    if (event.key.keysym.sym == SDLK_3)
                        Funkcja3();
                    if (event.key.keysym.sym == SDLK_4)
                        Funkcja4();
                    if (event.key.keysym.sym == SDLK_5)
                        Funkcja5();
                    if (event.key.keysym.sym == SDLK_l)
                        Linia(rand()%width,rand()%height,rand()%width,rand()%height, 255, 128, 255);
                    if (event.key.keysym.sym == SDLK_o)
                        Okrag(rand()%width,rand()%height,rand()%200, 128, 255, 255);
                    if (event.key.keysym.sym == SDLK_e)
                        Elipsa(rand()%width,rand()%height,rand()%200,rand()%200, 255, 255, 128);

                    if (event.key.keysym.sym == SDLK_b)
                        czyscEkran(10, 0, 0);          break;
                     }
            } // end switch
        } // end of message processing

    } // end main loop


    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
