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

typedef struct Palyameret{
    int magassag;
    int szelesseg;
}Palyameret;

typedef struct Jatek{
    Palyameret meretek;
    Mezoadatok **palya;
    int bombakszama;
    bool vege;
    bool nyert;
}Jatek;

typedef enum Jeltype{flag, dig}Jeltype;

typedef struct Jeloles{
    int y, x;
    Jeltype jel;
}Jeloles;

typedef struct ListaJatekosok{
    int ido;
    char nev[21];
    struct ListaJatekosok *kov;
}ListaJatekosok;

typedef struct ListaPalya{
    int x, y, bombaszam;
    ListaJatekosok *jatekosok;
    struct ListaPalya *kovpalya;
}ListaPalya;

typedef enum MezoKepek{
    bomba0, bomba1, bomba2, bomba3, bomba4, bomba5,
    bomba6, bomba7, bomba8, fedettkep, zaszlokep, bombakep
}MezoKepek;

#endif //AKNAKERESO_STRUCTOK_H
