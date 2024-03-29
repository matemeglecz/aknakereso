#ifndef SDL_FGVEK_H_INCLUDED
#define SDL_FGVEK_H_INCLUDED
#include "structok.h"

void sdl_init(char const *felirat, SDL_Window **pwindow, SDL_Renderer **prenderer, int szeles, int magas);
int jelolsdl(Jatek *j, Jeloles *aktjeloles);
void palyarajzol(SDL_Renderer *renderer, Jatek j);
int almenu(SDL_Renderer *renderer, Palyaparameterek *parameterek);
int fomenu(SDL_Renderer *renderer, SDL_Window *window);
int nyert_rajzol(SDL_Renderer *renderer, SDL_Window *window, Eredmenyadatok *eredmeny);
int jatekvege_almenu(SDL_Renderer *renderer, SDL_Window *window);
int veszt_rajzol(SDL_Renderer *renderer, SDL_Window *window);
int ranglistakiir_sdl(ListaPalya *eleje, Palyaparameterek parameterek, SDL_Renderer *renderer);

#endif // SDL_FGVEK_H_INCLUDED
