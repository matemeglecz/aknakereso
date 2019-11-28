#ifndef AKNAKERESO_STRUCTOK_H
#define AKNAKERESO_STRUCTOK_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum Bomba_e{
    bomba, nembomba
}Bomba_e;

typedef enum Mezoallapot{
    jelolt, fedett, nyitott
}Mezoallapot;

typedef struct Mezoadatok{
    Bomba_e bomba_e;
    Mezoallapot allapot;
    int nearbomb;
}Mezoadatok;

typedef struct Palyaparameterek{
    int magassag;
    int szelesseg;
    int bombaszam;
}Palyaparameterek;

typedef enum Jatekallapot{
    jatekban, nyert, vesztett
}Jatekallapot;

typedef struct Jatek{
    Palyaparameterek parameterek;
    Mezoadatok **palya;
    Jatekallapot jatek_allapot;
}Jatek;

typedef enum Jeltype{flag, dig}Jeltype;

typedef struct Jeloles{
    int y, x;
    Jeltype jel;
}Jeloles;

typedef struct Eredmenyadatok{
    int ido;
    char nev[21];
}Eredmenyadatok;

typedef struct ListaJatekosok{
    Eredmenyadatok adatok;
    struct ListaJatekosok *kov;
}ListaJatekosok;

typedef struct ListaPalya{
    Palyaparameterek palya_parameterek;
    ListaJatekosok *jatekosok;
    struct ListaPalya *kovpalya;
}ListaPalya;

typedef enum MezoKepek{
    bomba0, bomba1, bomba2, bomba3, bomba4, bomba5,
    bomba6, bomba7, bomba8, fedettkep, zaszlokep, bombakep
}MezoKepek;

#endif //AKNAKERESO_STRUCTOK_H
