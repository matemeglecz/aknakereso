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
        while(mozgo->kov!=NULL && mozgo->adatok.ido <= uj->adatok.ido){
            lemarado=mozgo;
            mozgo=mozgo->kov;
        }
        if(mozgo->kov==NULL && mozgo->adatok.ido <= uj->adatok.ido){
            uj->kov=NULL;
            mozgo->kov=uj;
        }else{
            if(lemarado!=NULL){
                uj->kov=mozgo;
                lemarado->kov=uj;
            }else if(mozgo->adatok.ido > uj->adatok.ido){
                uj->kov=eleje;
                eleje=uj;
            }
        }
    }
    return eleje;
}

static ListaPalya* ujlistapalya(Palyaparameterek aktparameterek, Eredmenyadatok akteredmeny){
    ListaPalya *ujpalya=(ListaPalya*)malloc(sizeof(ListaPalya));
    ujpalya->palya_parameterek.szelesseg=aktparameterek.szelesseg;
    ujpalya->palya_parameterek.magassag=aktparameterek.magassag;
    ujpalya->palya_parameterek.bombaszam=aktparameterek.bombaszam;
    ujpalya->kovpalya=NULL;
    ujpalya->jatekosok=(ListaJatekosok*)malloc(sizeof(ListaJatekosok));
    ujpalya->jatekosok->adatok.ido=akteredmeny.ido;
    strcpy(ujpalya->jatekosok->adatok.nev, akteredmeny.nev);
    ujpalya->jatekosok->kov=NULL;
    return ujpalya;
}

static ListaPalya* vanranglista(ListaPalya *eleje, Palyaparameterek aktparameterek, Eredmenyadatok akteredmeny){
    ListaPalya* mozgoPalya=eleje;
    while(!(mozgoPalya->palya_parameterek.szelesseg == aktparameterek.szelesseg &&
            mozgoPalya->palya_parameterek.magassag == aktparameterek.magassag &&
            mozgoPalya->palya_parameterek.bombaszam == aktparameterek.bombaszam) &&
            mozgoPalya->kovpalya != NULL){
        mozgoPalya=mozgoPalya->kovpalya;
    }

    if(mozgoPalya->palya_parameterek.szelesseg==aktparameterek.szelesseg && mozgoPalya->palya_parameterek.magassag==aktparameterek.magassag && mozgoPalya->palya_parameterek.bombaszam==aktparameterek.bombaszam){
        ListaJatekosok *ujjatekos=(ListaJatekosok *)malloc(sizeof(ListaJatekosok));
        ujjatekos->adatok.ido=akteredmeny.ido;
        strcpy(ujjatekos->adatok.nev, akteredmeny.nev);
        mozgoPalya->jatekosok=listarendez(mozgoPalya->jatekosok, ujjatekos);
    }
    else{ //ha még nincs ilyen pályamérethez tartozó lista
        mozgoPalya->kovpalya=ujlistapalya(aktparameterek, akteredmeny);
    }
    return eleje;
}

static ListaPalya* fajlbol_listaba(FILE *fp, ListaPalya *eleje){
    Palyaparameterek aktparameterek;
    Eredmenyadatok akteredmeny;
    while(fscanf(fp," %d  %d  %d  %d  %[^\n]", &aktparameterek.szelesseg, &aktparameterek.magassag, &aktparameterek.bombaszam, &akteredmeny.ido, akteredmeny.nev) >0){
        if(eleje==NULL){
            eleje=ujlistapalya(aktparameterek, akteredmeny);
        }
        else
            eleje=vanranglista(eleje, aktparameterek, akteredmeny);
    }
    return eleje;
}

void static fajlba_lista(FILE *fp, ListaPalya *eleje){
    for(ListaPalya *mozgoPalya=eleje; mozgoPalya!=NULL; mozgoPalya=mozgoPalya->kovpalya){
        int helyezes=0;// mindig csak a top 10 lesz visszamentve, a többi úgyis jelentéktelen
        for(ListaJatekosok *mozgoJatekosok=mozgoPalya->jatekosok; mozgoJatekosok!=NULL && helyezes<10; mozgoJatekosok=mozgoJatekosok->kov){
            fprintf(fp, "%d\t%d\t%d\t%d\t%s\n", mozgoPalya->palya_parameterek.szelesseg, mozgoPalya->palya_parameterek.magassag, mozgoPalya->palya_parameterek.bombaszam, mozgoJatekosok->adatok.ido, mozgoJatekosok->adatok.nev);
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

ListaPalya* ranglistabair(ListaPalya *ranglista, Palyaparameterek parameterek, Eredmenyadatok eredmeny){
    if(ranglista==NULL)
        ranglista=ujlistapalya(parameterek, eredmeny);
    else
        ranglista=vanranglista(ranglista, parameterek, eredmeny);

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
