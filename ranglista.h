#ifndef RANGLSITA_H_INCLUDED
#define RANGLSITA_H_INCLUDED
#include "structok.h"

ListaPalya* ranglistaolv();
void ranglista_ment(ListaPalya *ranglista);
ListaPalya* ranglistabair(ListaPalya *ranglista, int x, int y, int bombaszam, int ido, char *nev);
void felszabPalyalista(ListaPalya *eleje);

#endif // RANGLSITA_H_INCLUDED
