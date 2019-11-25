#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "structok.h"
#include "jatek_fgvek.h"
#include "ranglista.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "sdl_fgvek.h"
#include <SDL_image.h>

#include "debugmalloc.h"

int ujjatek(ListaPalya **ranglista, SDL_Renderer *renderer, SDL_Window *window){
    Jatek j1;
    j1.vege=false;
    j1.nyert=false;
    time_t start,end;
    double diff;
    Jeloles aktjel;

    if(almenu(renderer, &j1.meretek.szelesseg, &j1.meretek.magassag, &j1.bombakszama)==1)//kilépés ha 1
        return 1;
    SDL_DestroyWindow(window);
    sdl_init("Aknakereso", &window, &renderer, j1.meretek.szelesseg*20, j1.meretek.magassag*20);

    palyaletrehoz(&j1);
    palyarajzol(renderer, j1);

    //első jelölés
    if(jelolsdl(&j1, &aktjel)==1){//kilépés ha 1
        free(j1.palya[0]);
        free(j1.palya);
        return 1;
    }

    while(aktjel.jel!=dig){ //csak ásásra generálódik a pálya, a flagek nem érdekesek
       palyarajzol(renderer, j1);
       if(jelolsdl(&j1, &aktjel)==1){
            free(j1.palya[0]);
            free(j1.palya);
            return 1;
        }
    }

    time(&start);//idő az első ásással indul
    bombatgeneral(&j1);
    allapotvaltoztat(aktjel, &j1);
    palyarajzol(renderer, j1);
    j1.nyert=nyert_e(&j1); //akkor lenne lehetséges ha lehetne 0 bomba de azért a biztonság kedvéért itt van

    while(!j1.vege){
        if(jelolsdl(&j1, &aktjel)==1){//kilépés ha 1
            free(j1.palya[0]);
            free(j1.palya);
            return 1;
        }

        allapotvaltoztat(aktjel, &j1);
        j1.nyert=nyert_e(&j1);
        palyarajzol(renderer, j1);
    }

    palyarajzol(renderer, j1);
    time(&end);
    diff=difftime(end, start);

    if(j1.nyert){
        char nev[21];
        if(nyert_rajzol(renderer, window, nev, diff)==1){//kilépés ha 1
            free(j1.palya[0]);
            free(j1.palya);
            return 1;
        }

        if(*ranglista==NULL)
            *ranglista=ranglistaolv();

        *ranglista=ranglistabair(*ranglista, j1.meretek.szelesseg, j1.meretek.magassag, j1.bombakszama, diff, nev);//történhetne a név megadás után, most a tovább gomb után ment
        ranglista_ment(*ranglista);

    }
    else if(veszt_rajzol(renderer, window)==1){//kilépés ha 1
            free(j1.palya[0]);
            free(j1.palya);
            return 1;
    }
    SDL_DestroyWindow(window);
    sdl_init("Aknakereso", &window, &renderer, 800, 600);

    int almenu_allapot=jatekvege_almenu(renderer, window);
    if(almenu_allapot==2){
            SDL_DestroyWindow(window);
            sdl_init("Ranglista", &window, &renderer, 800, 600);
            if(*ranglista==NULL)
                *ranglista=ranglistaolv();
            if(ranglistakiir_sdl(*ranglista, j1.meretek.szelesseg, j1.meretek.magassag, j1.bombakszama, renderer)==1){//kilépés ha 1
                free(j1.palya[0]);
                free(j1.palya);
                return 1;
            }
    }
    else if(almenu_allapot==1){//kilépés ha 1
            free(j1.palya[0]);
            free(j1.palya);
            return 1;
    }

    SDL_DestroyWindow(window);
    free(j1.palya[0]);
    free(j1.palya);
    return 0;
}
