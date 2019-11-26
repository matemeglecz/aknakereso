#include "jatek_fgvek.h"
#include "structok.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "debugmalloc.h"

void static felderit(int y, int x, Jatek *j){
    if(j->palya[y][x].nearbomb!=0)
        return;
    for(int w=x-1; w<=x+1; w++){
        for(int h=y-1; h<=y+1;h++){
            if(!(h<0 || h>j->parameterek.magassag-1 || w<0 || w>j->parameterek.szelesseg-1) && j->palya[h][w].bomba_e == nembomba && j->palya[h][w].allapot!=nyitott){
                if(j->palya[h][w].nearbomb==0){
                    j->palya[h][w].allapot=nyitott;
                    felderit(h, w, j);
                }
                else if(j->palya[y][x].nearbomb==0){ //0-ás terület levő számok felderítése
                    j->palya[h][w].allapot=nyitott;
                }
            }
        }
    }
    return;
}

void allapotvaltoztat(Jeloles aktjeloles, Jatek *j){
    const int y=(aktjeloles.y);
    const int x=(aktjeloles.x);
    if(aktjeloles.jel==dig){
        if(j->palya[y][x].bomba_e==bomba){
            j->jatek_allapot=vesztett;
        }
        else{
            felderit(y, x, j);
        }
    }
}

void static mezoknovelese(Jatek *j,int y,int x) {
    for(int w=x-1; w<=x+1; w++){
        for(int h=y-1; h<=y+1;h++){
            if(!(h<0 || h>j->parameterek.magassag-1 || w<0 || w>j->parameterek.szelesseg-1)){
                j->palya[h][w].nearbomb++;
            }
        }
    }
}

void bombatgeneral(Jatek *j){
    int sikeresgen=0;
    while(sikeresgen < j->parameterek.bombaszam){
        int randommagassag=rand()%j->parameterek.magassag;
        int randomszelesseg=rand()%j->parameterek.szelesseg;
        if(j->palya[randommagassag][randomszelesseg].bomba_e!=bomba && j->palya[randommagassag][randomszelesseg].allapot!=nyitott){ //nyitott vizsgálat az első kettintás miatt kell
            j->palya[randommagassag][randomszelesseg].bomba_e = bomba;
            mezoknovelese(j, randommagassag, randomszelesseg);
            sikeresgen++;
        }
    }
}

static void palyafoglal(Jatek* j){
    j->palya=(Mezoadatok **)malloc(j->parameterek.magassag*sizeof(Mezoadatok*));
    j->palya[0]=(Mezoadatok *)malloc(j->parameterek.magassag*sizeof(Mezoadatok)*(j->parameterek.szelesseg));
    for (int y = 1; y < j->parameterek.magassag; ++y)
        j->palya[y] = j->palya[0] + (y*j->parameterek.szelesseg);
}

static void lenullaz(Jatek*j){
    for(int y=0; y<j->parameterek.magassag; y++){
        for(int x=0; x<j->parameterek.szelesseg;x++){
            j->palya[y][x].nearbomb=0;
            j->palya[y][x].bomba_e=nembomba;
            j->palya[y][x].allapot=fedett;
        }
    }
}

void palyaletrehoz(Jatek *j){
    srand(time(0));
    palyafoglal(j);
    lenullaz(j);
}

void nyert_e(Jatek *j){//akkor nyert ha minden mező fel van derítve ami nem bomba és minden bomba be van jelölve
    for(int x=0; x<j->parameterek.szelesseg; x++){
        for(int y=0; y<j->parameterek.magassag; y++){
            if((j->palya[y][x].bomba_e==nembomba && j->palya[y][x].allapot!=nyitott )||
               (j->palya[y][x].bomba_e==bomba && j->palya[y][x].allapot!=jelolt))
                return;//játékban maradunk
        }
    }
    j->jatek_allapot=nyert;
    return;
}
