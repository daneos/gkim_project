#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include<iostream>

#include <SDL/SDL.h>

SDL_Color getPixel (int x, int y, SDL_Surface* screen) {
    SDL_Color color ;
    Uint32 col = 0 ;
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
    return ( color ) ;
}

struct conv_bmp {
    int height;
    int width;
    Uint8 bitsperpixel;
    Uint8 red_color;    //powinna byc tablca ktora bedzie trzymala RGB kazdego piksela
    Uint8 green_color;
    Uint8 blue_color;
};


int main ( int argc, char** argv )
{
    SDL_Surface* bmp = SDL_LoadBMP("cb.bmp");
    conv_bmp new_bmp;
    new_bmp.height = bmp->h;
    new_bmp.width = bmp->w;
    new_bmp.bitsperpixel = 4;
    //for(int i=0;i<bmp->h;i++) {
      //  for(int j=0;i<bmp->w;j++) {
            SDL_Color color = getPixel(50,50,bmp);
            new_bmp.red_color = color.r/16;
            new_bmp.green_color = color.g/16;
            new_bmp.blue_color = color.b/16;
            printf("Pixel Color -> R: %d,  G: %d,  B: %d, \n", new_bmp.red_color, new_bmp.green_color, new_bmp.blue_color);   //ta wersja nie dziala mimo podstawienia i tych samych typow Uint8
            printf("Pixel Color -> R: %d,  G: %d,  B: %d, \n", color.r/16, color.g/16, color.b/16); //ta wersja dziala
       // }
  //  }

  //ogolnie mysle ze bedzie mozna tak zrobic. dostaniemy strukture, do ktorej dolozymy co tam bedzie potrzeba w algorytmach kompresjii
  //potrzebuje zeby ktos rzucil okiem, bo ja juz wychodze z roboty czemu nie chce mi wypisywac wartosci ze struktury po przypisaniu.
  //ps. dzielenie przez 16 powoduje skrocenie bitow do 4 do w zasadzie zalatwia sprawe ;]
    printf("Exited cleanly\n");
    return 0;
}

