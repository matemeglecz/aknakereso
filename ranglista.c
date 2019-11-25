#include "ranglista.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "structok.h"

#include "debugmalloc.h"

static ListaJatekosok* listarendez(ListaJatekosok *eleje, ListaJatekosok *uj){
    if(eleje==NULL){
        eleje=uj;
        uj->kov=NULL;
    }
    else{
        ListaJatekosok *lemarado=NULL;
        ListaJatekosok *mozgo=eleje;
        while(mozgo->kov!=NULL && mozgo->ido <= uj->ido){
            lemarado=mozgo;
            mozgo=mozgo->kov;
        }
        if(mozgo->kov==NULL && mozgo->ido <= uj->ido){
            uj->kov=NULL;
            mozgo->kov=uj;
        }else{
            if(lemarado!=NULL){
                uj->kov=mozgo;
                lemarado->kov=uj;
            }else if(mozgo->ido > uj->ido){
                uj->kov=eleje;
                eleje=uj;
            }
        }
    }
    return eleje;
}

static ListaPalya* ujlistapalya(int aktx, int akty, int aktbombaszam, int aktido, char *nev){
    ListaPalya *ujpalya=(ListaPalya*)malloc(sizeof(ListaPalya));
    ujpalya->x=aktx;
    ujpalya->y=akty;
    ujpalya->bombaszam=aktbombaszam;
    ujpalya->kovpalya=NULL;
    ujpalya->jatekosok=(ListaJatekosok*)malloc(sizeof(ListaJatekosok));
    ujpalya->jatekosok->ido=aktido;
    strcpy(ujpalya->jatekosok->nev, nev);
    ujpalya->jatekosok->kov=NULL;
    return ujpalya;
}

static ListaPalya* vanranglista(ListaPalya *eleje, int aktx, int akty, int aktbombaszam, int aktido, char *nev){
    ListaPalya* mozgoPalya=eleje;
    while(!(mozgoPalya->x == aktx && mozgoPalya->y == akty && mozgoPalya->bombaszam == aktbombaszam) && mozgoPalya->kovpalya != NULL){
        mozgoPalya=mozgoPalya->kovpalya;
    }

    if(mozgoPalya->x==aktx && mozgoPalya->y==akty && mozgoPalya->bombaszam==aktbombaszam){
        ListaJatekosok *ujjatekos=(ListaJatekosok *)malloc(sizeof(ListaJatekosok));
        ujjatekos->ido=aktido;
        strcpy(ujjatekos->nev, nev);
        mozgoPalya->jatekosok=listarendez(mozgoPalya->jatekosok, ujjatekos);
    }
    else{ //ha még nincs ilyen pályamérethez tartozó lista
        mozgoPalya->kovpalya=ujlistapalya(aktx, akty, aktbombaszam, aktido, nev);
    }
    return eleje;
}

static ListaPalya* fajlbol_listaba(FILE *fp, ListaPalya *eleje){
    int aktx, akty ,aktbombaszam, aktido;
    char nev[21];
    while(fscanf(fp," %d  %d  %d  %d  %[^\n]", &aktx, &akty, &aktbombaszam, &aktido, nev) >0){
        if(eleje==NULL){
            eleje=ujlistapalya(aktx, akty, aktbombaszam, aktido, nev);
        }
        else
            eleje=vanranglista(eleje, aktx, akty, aktbombaszam, aktido, nev);
    }
    return eleje;
}

void static fajlba_lista(FILE *fp, ListaPalya *eleje){
    for(ListaPalya *mozgoPalya=eleje; mozgoPalya!=NULL; mozgoPalya=mozgoPalya->kovpalya){
        int helyezes=0;// mindig csak a top 10 lesz visszamentve, a többi úgyis jelentéktelen
        for(ListaJatekosok *mozgoJatekosok=mozgoPalya->jatekosok; mozgoJatekosok!=NULL && helyezes<10; mozgoJatekosok=mozgoJatekosok->kov){
            fprintf(fp, "%d\t%d\t%d\t%d\t%s\n", mozgoPalya->x, mozgoPalya->y, mozgoPalya->bombaszam, mozgoJatekosok->ido, mozgoJatekosok->nev);
            helyezes++;
        }
    }
}

void static felszabJatekosoklista(ListaJatekosok *eleje){
    ListaJatekosok *mozgo=eleje;
    while(mozgo!=NULL){
        ListaJatekosok *kov=mozgo->kov;
        free(mozgo);
        mozgo=kov;
    }
}

void felszabPalyalista(ListaPalya *eleje){
    ListaPalya *mozgo=eleje;
    while(mozgo!=NULL){
        ListaPalya *kov=mozgo->kovpalya;
        felszabJatekosoklista(mozgo->jatekosok);
        free(mozgo);
        mozgo=kov;
    }
}

ListaPalya* ranglistaolv(){
    FILE *fp;
    ListaPalya *eleje=NULL;
    fp = fopen("ranglista.txt", "rt");
    if (fp == NULL) {
    perror("Fájl megnyitása sikertelen");
    return eleje;    /* nem folytathatjuk! */
    }

    eleje=fajlbol_listaba(fp, eleje);

    fclose(fp);
    return eleje;
}

ListaPalya* ranglistabair(ListaPalya *ranglista, int x, int y, int bombaszam, int ido, char *nev){
    if(ranglista==NULL)
        ranglista=ujlistapalya(x, y, bombaszam, ido, nev);
    else
        ranglista=vanranglista(ranglista, x, y, bombaszam, ido, nev);

    return ranglista;
}

void ranglista_ment(ListaPalya *ranglista){
    FILE *fp;
    fp = fopen("ranglista.txt", "wt");
    if (fp == NULL) {
    perror("Fájl megnyitása sikertelen");
    return;    // nem folytathatjuk!
    }

    fajlba_lista(fp, ranglista);

    fclose(fp);
}
