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
            if(!(h<0 || h>j->meretek.magassag-1 || w<0 || w>j->meretek.szelesseg-1) && j->palya[h][w].bomba_e == nembomba && j->palya[h][w].allapot==fedett){
                if(j->palya[h][w].nearbomb==0){
                    j->palya[h][w].allapot=nyitott;
                    felderit(h, w, j);
                }
                else if(j->palya[y][x].nearbomb==0){
                    j->palya[h][w].allapot=nyitott;
                    /*if(j->palya[h][w].nearbomb==0)
                        felderit(h, w, j);*/
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
            j->vege=true;
        }
        else{
            felderit(y, x, j);
        }
    }
}

void static mezoknovelese(Jatek *j,int y,int x) {
    for(int w=x-1; w<=x+1; w++){
        for(int h=y-1; h<=y+1;h++){
            if(!(h<0 || h>j->meretek.magassag-1 || w<0 || w>j->meretek.szelesseg-1)){
                j->palya[h][w].nearbomb++;
            }
        }
    }
}

void bombatgeneral(Jatek *j){
    int sikeresgen=0;
    while(sikeresgen<j->bombakszama){
        int randommagassag=rand()%j->meretek.magassag;
        int randomszelesseg=rand()%j->meretek.szelesseg;
        if(j->palya[randommagassag][randomszelesseg].bomba_e!=bomba && j->palya[randommagassag][randomszelesseg].allapot!=nyitott){
            j->palya[randommagassag][randomszelesseg].bomba_e = bomba;
            mezoknovelese(j, randommagassag, randomszelesseg);
            sikeresgen++;
        }
    }

    if(sikeresgen==j->bombakszama){  //csak tesztelésre
    for(int y=0; y<j->meretek.magassag; y++){
        for(int x=0; x<j->meretek.szelesseg;x++){
           if(j->palya[y][x].bomba_e==bomba)
                printf("X ");
           else
                printf("%d ", j->palya[y][x].nearbomb);
        }
        printf("\n");
    }
    }
}

static void palyafoglal(Jatek* j){
    j->palya=(Mezoadatok **)malloc(j->meretek.magassag*sizeof(Mezoadatok*));
    j->palya[0]=(Mezoadatok *)malloc(j->meretek.magassag*sizeof(Mezoadatok)*(j->meretek.szelesseg));
    for (int y = 1; y < j->meretek.magassag; ++y)
        j->palya[y] = j->palya[0] + (y*j->meretek.szelesseg);
}

static void lenullaz(Jatek*j){
    for(int y=0; y<j->meretek.magassag; y++){
        for(int x=0; x<j->meretek.szelesseg;x++){
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

bool nyert_e(Jatek *j){
    for(int x=0; x<j->meretek.szelesseg; x++){
        for(int y=0; y<j->meretek.magassag; y++){
            if((j->palya[y][x].bomba_e==nembomba && j->palya[y][x].allapot!=nyitott )||
               (j->palya[y][x].bomba_e==bomba && j->palya[y][x].allapot!=jelolt))
                return false;
        }
    }
    j->vege=true;
    return true;
}
