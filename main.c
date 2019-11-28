#include <stdio.h>
#include "structok.h"
#include "ujjatek.h"
#include "ranglista.h"
#include "sdl_fgvek.h"
#include "debugmalloc.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>


int main(int argc, char *argv[]){
    SDL_Window *window;
    SDL_Renderer *renderer;
    ListaPalya *ranglista=NULL;
    Palyaparameterek parameterek;

    while(1){
        sdl_init("Aknakereso", &window, &renderer, 800, 600);
        switch(fomenu(renderer, window)){
            case 1://ujjatek
                if(ujjatek(&ranglista, renderer, window)==1){//kilépés
                    felszabPalyalista(ranglista);
                    return 0;
                }
                break;
            case 2://ranglista
                if(almenu(renderer, &parameterek)==1){
                    felszabPalyalista(ranglista);
                    return 0;
                }

                if(ranglista==NULL)
                    ranglista=ranglistaolv();
                if(ranglistakiir_sdl(ranglista, parameterek, renderer)==1){//kilépés
                    felszabPalyalista(ranglista);
                    return 0;
                }
                SDL_DestroyWindow(window);
                break;
            case 3://kilépés
                SDL_Quit();
                felszabPalyalista(ranglista);
                return 0;
        }
    }
return 0;
}
