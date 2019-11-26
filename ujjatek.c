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
    Jatek j;
    j.jatek_allapot=jatekban;
    time_t start,end;
    double diff;
    Jeloles aktjel;

    if(almenu(renderer, &j.meretek.szelesseg, &j.meretek.magassag, &j.bombakszama)==1)//kilépés ha 1
        return 1;
    SDL_DestroyWindow(window);
    sdl_init("Aknakereso", &window, &renderer, j.meretek.szelesseg*20, j.meretek.magassag*20);

    palyaletrehoz(&j);
    palyarajzol(renderer, j);

    //első jelölés
    if(jelolsdl(&j, &aktjel)==1){//kilépés ha 1
        free(j.palya[0]);
        free(j.palya);
        return 1;
    }

    while(aktjel.jel!=dig){ //csak ásásra generálódik a pálya, a flagek nem érdekesek
       palyarajzol(renderer, j);
       if(jelolsdl(&j, &aktjel)==1){
            free(j.palya[0]);
            free(j.palya);
            return 1;
        }
    }

    time(&start);//idő az első ásással indul
    bombatgeneral(&j);
    allapotvaltoztat(aktjel, &j);
    palyarajzol(renderer, j);
    nyert_e(&j); //akkor lenne lehetséges ha lehetne 0 bomba de azért a biztonság kedvéért itt van

    while(j.jatek_allapot==jatekban){
        if(jelolsdl(&j, &aktjel)==1){//kilépés ha 1
            free(j.palya[0]);
            free(j.palya);
            return 1;
        }

        allapotvaltoztat(aktjel, &j);
        nyert_e(&j);
        palyarajzol(renderer, j);
    }

    palyarajzol(renderer, j);
    time(&end);
    diff=difftime(end, start);

    if(j.jatek_allapot==nyert){
        char nev[21];
        if(nyert_rajzol(renderer, window, nev, diff)==1){//kilépés ha 1
            free(j.palya[0]);
            free(j.palya);
            return 1;
        }

        if(*ranglista==NULL)
            *ranglista=ranglistaolv();

        *ranglista=ranglistabair(*ranglista, j.meretek.szelesseg, j.meretek.magassag, j.bombakszama, diff, nev);//történhetne a név megadás után, most a tovább gomb után ment
        ranglista_ment(*ranglista);

    }
    else if(veszt_rajzol(renderer, window)==1){//kilépés ha 1
            free(j.palya[0]);
            free(j.palya);
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
            if(ranglistakiir_sdl(*ranglista, j.meretek.szelesseg, j.meretek.magassag, j.bombakszama, renderer)==1){//kilépés ha 1
                free(j.palya[0]);
                free(j.palya);
                return 1;
            }
    }
    else if(almenu_allapot==1){//kilépés ha 1
            free(j.palya[0]);
            free(j.palya);
            return 1;
    }

    SDL_DestroyWindow(window);
    free(j.palya[0]);
    free(j.palya);
    return 0;
}
