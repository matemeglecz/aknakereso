#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL_image.h>
#include "structok.h"
#include "sdl_segedfgvek.h"

void szovegir(char *szoveg, SDL_Color szin, TTF_Font *font, SDL_Renderer *renderer, int szeles, int x, int y){ //a szeles a sav szelessege amin belül középre igazítva szeretnénk kiírni, x kezdőpont szélességben, y ugyanígy magasságban
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    felirat = TTF_RenderUTF8_Blended(font, szoveg, szin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_Rect hova = { 0, 0, 0, 0};
    hova.x = x + szeles/2-felirat->w/2;
    hova.y = y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            // A feldolgozott szoveg bemenete
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }

                // Az eddigi szerkesztes torolheto
                composition[0] = '\0';
                break;

            // Szoveg szerkesztese
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                // visszatesszuk a sorba ezt az eventet, mert sok mindent nem tudunk vele kezdeni
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }

    //igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus
    SDL_StopTextInput();
    return enter;
}

int beolvas(SDL_Renderer *renderer, int *adat, char *szoveg){
    char bemenet[10];

    TTF_Init();
    TTF_Font *fontszam = TTF_OpenFont("LiberationSerif-Regular.ttf", 150);
    if (!fontszam) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *fontbetuk = TTF_OpenFont("LiberationSerif-Regular.ttf", 100);
    if (!fontbetuk) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Rect xteglalap= { 200, 200, 400, 200};


    SDL_Color feher = {255, 255, 255};
    szovegir(szoveg , feher, fontbetuk, renderer, 800, 0, 75);
    SDL_Color fekete = {0, 0, 0};

    bool sikeres_beolv=input_text(bemenet, 10, xteglalap, feher, fekete, fontszam, renderer);
    *adat=atoi(bemenet);

    TTF_CloseFont(fontszam);
    TTF_CloseFont(fontbetuk);
    SDL_RenderClear(renderer);

    if(sikeres_beolv)
        return 0;
    else return 1;
}

enum { MERET = 20 };
void mezo_rajzol(SDL_Renderer *renderer, SDL_Texture *mezok, MezoKepek melyik, int x, int y) {
    /* a forras kepbol ezekrol a koordinatakrol, ilyen meretu reszletet masolunk. */
    SDL_Rect src = { (melyik % 6) * 20, (melyik / 6) * 20, MERET, MERET };
    /* a cel kepre, ezekre a koordinatakra masoljuk */
    SDL_Rect dest = { x*MERET, y*MERET, MERET, MERET };
    /* kepreszlet masolasa */
    SDL_RenderCopy(renderer, mezok, &src, &dest);
}
