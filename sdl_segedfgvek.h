#ifndef SDL_SEGEDFGVEK_H_INCLUDED
#define SDL_SEGEDFGVEK_H_INCLUDED

int beolvas_rajzol(SDL_Renderer *renderer, int *adat, char* szoveg);
void szovegir(char *szoveg, SDL_Color szin, TTF_Font *font, SDL_Renderer *renderer, int szeles, int x, int y);
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);
void mezo_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, MezoKepek melyik, int x, int y);

#endif // SDL_SEGEDFGVEK_H_INCLUDED
