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

SDL_Color feher = {255, 255, 255};
SDL_Color fekete = {0, 0, 0};
SDL_Color piros = {255, 0, 0};
SDL_Color vilagos_zold = {102, 255, 51};

void sdl_init(char const *felirat, SDL_Window **pwindow, SDL_Renderer **prenderer, int szeles, int magas) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

int fomenu(SDL_Renderer *renderer, SDL_Window *window){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 60);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    //SDL_Color feher = {255, 255, 255};

    rectangleRGBA(renderer, 200, 125, 600, 225, 255, 255, 255, 255);
    szovegir("Új játék", feher, font, renderer, 800, 0, 135);
    rectangleRGBA(renderer, 200, 250, 600, 350, 255, 255, 255, 255);
    szovegir("Ranglista", feher, font, renderer, 800, 0,  260);
    rectangleRGBA(renderer, 200, 375, 600, 475, 255, 255, 255, 255);
    szovegir("Kilépés", feher, font, renderer, 800, 0, 385);



    /* az elvegzett rajzolasok a kepernyore */

    SDL_RenderPresent(renderer);

    SDL_Event ev;
    while(1){
        SDL_WaitEvent(&ev);
        int x, y;
        switch(ev.type){
        case SDL_QUIT:
            //SDL_Quit();
            return 3;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<=600 && x >= 200 && y>=125 && y <=225 && ev.button.button==SDL_BUTTON_LEFT)
                return 1;
            if(x<=600 && x >= 200 && y>=250 && y <=350 && ev.button.button==SDL_BUTTON_LEFT)
                return 2;
            if(x<=600 && x >= 200 && y>=375 && y <=475 && ev.button.button==SDL_BUTTON_LEFT)
                return 3;
            break;
            }
        }

    TTF_CloseFont(font);
}

int almenu(SDL_Renderer *renderer, int *xi, int *yi, int *bombaszami, bool jatekbol){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 60);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    //SDL_Color feher = {255, 255, 255};

    rectangleRGBA(renderer, 200, 63, 600, 163, 255, 255, 255, 255);
    szovegir("Könnyű", feher, font, renderer, 800, 0, 73);
    rectangleRGBA(renderer, 200, 188, 600, 288, 255, 255, 255, 255);
    szovegir("Közepes", feher, font, renderer, 800, 0, 198);
    rectangleRGBA(renderer, 200, 312, 600, 412, 255, 255, 255, 255);
    szovegir("Nehéz", feher, font, renderer, 800, 0, 332);
    rectangleRGBA(renderer, 200, 437, 600, 537, 255, 255, 255, 255);
    szovegir("Egyéni", feher, font, renderer, 800, 0, 447);

    SDL_RenderPresent(renderer);

    SDL_Event ev;
    bool sikereskatt=false;
    while(!sikereskatt){
        SDL_WaitEvent(&ev);
        int x, y;
        switch(ev.type){
        case SDL_QUIT:
            SDL_Quit();
            TTF_CloseFont(font);
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<=600 && x >= 200 && y>=63 && y <= 163 && ev.button.button==SDL_BUTTON_LEFT){
                *xi=8;
                *yi=8;
                *bombaszami=10;
                sikereskatt=true;}
            if(x<=600 && x >= 200 && y>=188 && y <= 288 && ev.button.button==SDL_BUTTON_LEFT){
                *xi=16;
                *yi=16;
                *bombaszami=40;
                sikereskatt=true;}
            if(x<=600 && x >= 200 && y>=312 && y <= 412 && ev.button.button==SDL_BUTTON_LEFT){
                *xi=30;
                *yi=16;
                *bombaszami=99;
                sikereskatt=true;}
            if(x<=600 && x >= 200 && y>=437 && y <= 537 && ev.button.button==SDL_BUTTON_LEFT){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);//az almenü nem törlődik ki, ezért kell

                if(jatekbol){
                    SDL_DisplayMode DM;
                    SDL_GetDesktopDisplayMode(0, &DM);

                    if(beolvas(renderer, xi, "X")==1){
                        TTF_CloseFont(font);
                        return 1;
                    }
                    while(*xi > DM.w/20 || *xi <= 0){
                        szovegir("Hibás bemenet.", piros, font, renderer, 800, 0, 450);
                        if(beolvas(renderer, xi, "X")==1){
                            TTF_CloseFont(font);
                            return 1;
                        }
                    }

                    if(beolvas(renderer, yi, "Y")==1){
                        TTF_CloseFont(font);
                        return 1;
                    }
                    while(*yi > DM.h/20 || *yi <=0){
                        szovegir("Hibás bemenet.", piros, font, renderer, 800, 0, 450);
                        if(beolvas(renderer, yi, "Y")==1){
                            TTF_CloseFont(font);
                            return 1;
                        }
                    }

                    if(beolvas(renderer, bombaszami, "Bombaszám")==1){
                        TTF_CloseFont(font);
                        return 1;
                    }
                    while(*bombaszami > (*xi)*(*yi) || *bombaszami==0){ //nem lehet 0, mert ha betűt kap akkor is 0-ra konvertálja, meg egyébként sincs sok ételme
                        szovegir("Hibás bemenet.", piros, font, renderer, 800, 0, 450);
                        if(beolvas(renderer, bombaszami, "Bombaszám")==1){
                            TTF_CloseFont(font);
                            return 1;
                        }
                    }
                }
                else{//nincsenek kezelve a betuk
                    TTF_CloseFont(font);
                    if(beolvas(renderer, xi, "X")==1)
                        return 1;

                    if(beolvas(renderer, yi, "Y")==1)
                        return 1;

                    if(beolvas(renderer, bombaszami, "Bombaszám")==1)
                        return 1;

                }sikereskatt=true;
                }
                break;
            }
    }
    TTF_CloseFont(font);
    return 0;
    }

void palyarajzol(SDL_Renderer *renderer, Jatek j){
    SDL_Texture *mezokep= IMG_LoadTexture(renderer, "mezok.png");
    if (mezokep == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for(int h=0; h<j.meretek.magassag; h++){
        for(int w=0; w<j.meretek.szelesseg; w++){
            //SDL_Rect celterulet = { w*MERET, h*MERET, MERET, MERET};
            switch(j.palya[h][w].allapot){
            case jelolt:
                mezo_rajzol(renderer, mezokep, zaszlokep, w, h);
                break;
            case fedett:
                mezo_rajzol(renderer, mezokep, fedettkep, w, h);
                break;
            case nyitott:
                if(j.palya[h][w].bomba_e==bomba)
                    mezo_rajzol(renderer, mezokep, bombakep, w, h);
                else
                    mezo_rajzol(renderer, mezokep, j.palya[h][w].nearbomb, w, h);
                break;
            }
            //SDL_RenderCopy(renderer, fedettkep, NULL, &celterulet);

        }
    }

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(mezokep);
}

int jelolsdl(Jatek *j, Jeloles *aktjeloles){

    //Jeloles aktjeloles;
    SDL_Event ev;
    bool sikereskatt=false;
    while(!sikereskatt){
        SDL_WaitEvent(&ev);
        switch(ev.type){
        case SDL_QUIT:
            SDL_Quit();
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            aktjeloles->x=ev.button.x/20;
            aktjeloles->y=ev.button.y/20;
            break;
        case SDL_MOUSEBUTTONUP:
            if(ev.button.button==SDL_BUTTON_LEFT){
                aktjeloles->jel=dig;
                sikereskatt=true;}
            if(ev.button.button==SDL_BUTTON_RIGHT){
                aktjeloles->jel=flag;
                sikereskatt=true;}
            break;
            }
        }

    if(aktjeloles->jel==dig){
        j->palya[aktjeloles->y][aktjeloles->x].allapot=nyitott;
    }
    else if(j->palya[aktjeloles->y][aktjeloles->x].allapot==fedett){
        j->palya[aktjeloles->y][aktjeloles->x].allapot=jelolt;
    }
    else if(j->palya[aktjeloles->y][aktjeloles->x].allapot==jelolt)
        j->palya[aktjeloles->y][aktjeloles->x].allapot=fedett;
    return 0;

}

int nyert_rajzol(SDL_Renderer *renderer, SDL_Window *window, char *nev, int diff){
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect teglalap;
    teglalap.x=w/12;
    teglalap.y=h/2-h/9;
    teglalap.w=5*w/6;
    teglalap.h=2*h/9;

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", h/6);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

     SDL_Texture *konfetti= IMG_LoadTexture(renderer, "konfetti.png");
    if (konfetti == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    //kirajzol
    /* a cel kepre, ezekre a koordinatakra masoljuk */
    SDL_Rect dest = { 0, 0, w, h };
    /* kepreszlet masolasa */
    SDL_RenderCopy(renderer, konfetti, NULL, &dest);

    //rectangleRGBA(renderer, 200, 125, 600, 225, 255, 255, 255, 255);
    //rectangleRGBA(renderer, 200, 125, 600, 225, 255, 255, 255, 255);
    szovegir("NYERTÉL", feher, font, renderer, w, 0, h/12);

    char szoveg_ido[16]="IDŐ: ";
    szovegir(szoveg_ido, feher, font, renderer, w, 0, teglalap.y+teglalap.h+h/60);
    char ido[5+1]; //egy napnál csak nem fut tovább a játék
    strcpy(szoveg_ido, itoa(diff, ido, 10));
    strcat(szoveg_ido, " másodperc");
    szovegir(szoveg_ido, feher, font, renderer, w, 0, teglalap.y+teglalap.h+h/30+h/6);

    input_text(nev, 20, teglalap, fekete, feher, font, renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


    SDL_RenderFillRect(renderer, &teglalap);
    rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x+teglalap.w, teglalap.y+teglalap.h, 255, 255, 255, 255);
    szovegir("TOVÁBB", feher, font, renderer, teglalap.w, teglalap.x, teglalap.y);
    SDL_RenderPresent(renderer);

    TTF_CloseFont(font);
    SDL_DestroyTexture(konfetti);

    SDL_Event ev;
    //bool sikereskatt=false;
    while(1){
        SDL_WaitEvent(&ev);
        int x, y;
        switch(ev.type){
        case SDL_QUIT:
            SDL_Quit();
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<= teglalap.x+teglalap.w && x >= teglalap.x && y>=teglalap.y && y <= teglalap.y+teglalap.h && ev.button.button==SDL_BUTTON_LEFT)
                return 0;
        }
    }
}

int jatekvege_almenu(SDL_Renderer *renderer, SDL_Window *window){
    int w, h;
    SDL_GetWindowSize(window, &w, &h);


    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 2*h/10);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    rectangleRGBA(renderer, w/10, h/7, 9*w/10, 3*h/7, 255, 255, 255, 255);
    szovegir("Ranglista", feher, font, renderer, w, 0, h/7);
    rectangleRGBA(renderer, w/10, 4*h/7, 9*w/10, 6*h/7, 255, 255, 255, 255);
    szovegir("Főmenü", feher, font, renderer, w, 0, 4*h/7);

    SDL_RenderPresent(renderer);
    TTF_CloseFont(font);

    SDL_Event ev;
    while(1){
        SDL_WaitEvent(&ev);
        int x, y;
        switch(ev.type){
        case SDL_QUIT:
            SDL_Quit();
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<= 9*w/10 && x >= w/10 && y>=h/7 && y <= 3*h/7 && ev.button.button==SDL_BUTTON_LEFT){
                return 2;
            }
            else if(x<= 9*w/10 && x >= w/10 && y>=4*h/7 && y <= 6*h/7 && ev.button.button==SDL_BUTTON_LEFT){
                return 0;
            }
        }
    }
}

int veszt_rajzol(SDL_Renderer *renderer, SDL_Window *window){
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", h/6);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    //szovegir("BUMM:(", fekete, font, renderer, w, 0, h/12);

    SDL_Texture *robbanas= IMG_LoadTexture(renderer, "robbanas.png");
    if (robbanas == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    //kirajzol
    /* a cel kepre, ezekre a koordinatakra masoljuk */
    SDL_Rect dest = { 0, 0, w, h };
    /* kepreszlet masolasa */
    SDL_RenderCopy(renderer, robbanas, NULL, &dest);


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect teglalap;
    teglalap.x=w/12;
    teglalap.y=h/2-h/9;
    teglalap.w=5*w/6;
    teglalap.h=2*h/9;

    SDL_RenderFillRect(renderer, &teglalap);
    rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x+teglalap.w, teglalap.y+teglalap.h, 255, 255, 255, 255);
    szovegir("TOVÁBB", feher, font, renderer, teglalap.w, teglalap.x, teglalap.y);
    SDL_RenderPresent(renderer);

    TTF_CloseFont(font);
    SDL_DestroyTexture(robbanas);

    SDL_Event ev;
    //bool sikereskatt=false;
    while(1){
        SDL_WaitEvent(&ev);
        int x, y;
        switch(ev.type){
        case SDL_QUIT:
            SDL_Quit();
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<= teglalap.x+teglalap.w && x >= teglalap.x && y>=teglalap.y && y <= teglalap.y+teglalap.h && ev.button.button==SDL_BUTTON_LEFT)
                return 0;
        }
    }
}

int ranglistakiir_sdl(ListaPalya *eleje, int x, int y, int bombaszam, SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const int w=800;
    const int h=600;
    TTF_Init();
    TTF_Font *font_h = TTF_OpenFont("LiberationSerif-Regular.ttf", h/14);
    if (!font_h) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *font_cim = TTF_OpenFont("LiberationSerif-Regular.ttf", h/7);
    if (!font_cim) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    szovegir("RANGLISTA", feher, font_cim, renderer, w, 0, 0);
    if(x==8 && y==8 && bombaszam==10)
        szovegir("Könnyű", feher, font_h, renderer, w, 0, h/7);
    else if(x==16 &&y==16 && bombaszam==40)
        szovegir("Közepes", feher, font_h, renderer, w, 0, h/7);
    else if(x==30 && y==16 && bombaszam==99)
        szovegir("Nehéz", feher, font_h, renderer, w, 0, h/7);
    else
        szovegir("Egyéb méretek", feher, font_h, renderer, w, 0, h/7);

    TTF_CloseFont(font_cim);

    if(eleje==NULL)
        szovegir("Még nincsenek eredmények.", feher, font_h, renderer, w, 0, h/2);
    else{
        int helyezes=0;
        ListaPalya *palyakeres=eleje;
        while(palyakeres->kovpalya!=NULL && !(palyakeres->x==x && palyakeres->y==y && palyakeres->bombaszam==bombaszam))
            palyakeres=palyakeres->kovpalya;
            if(palyakeres->x==x && palyakeres->y==y && palyakeres->bombaszam==bombaszam){
                for(ListaJatekosok *mozgo=palyakeres->jatekosok; mozgo!=NULL && helyezes<10; mozgo=mozgo->kov){
                    char helyezesstring[4];
                    itoa(helyezes+1, helyezesstring, 10);
                    strcat(helyezesstring, ".");
                    szovegir(helyezesstring, feher, font_h, renderer, 150, 0, (helyezes+3)*h/14);

                    char idostring[9];
                    itoa(mozgo->ido, idostring, 10);
                    strcat(idostring, " mp");
                    szovegir(idostring, feher, font_h,renderer, 250, 150, (helyezes+3)*h/14);

                    szovegir(mozgo->nev, feher, font_h,renderer, 400, 400, (helyezes+3)*h/14);
                    printf("%2d. %4d mp   %s\n", helyezes+1,mozgo->ido, mozgo->nev);
                    helyezes++;
                }
            }
            else szovegir("Még nincsenek eredmények.", feher, font_h, renderer, w, 0, h/2);
    }

    SDL_Rect teglalap;
    teglalap.x=3*w/4;
    teglalap.y=h-h/14;
    teglalap.w=w/4;
    teglalap.h=h/14;

    SDL_RenderFillRect(renderer, &teglalap);
    rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x+teglalap.w, teglalap.y+teglalap.h, 255, 255, 255, 255);
    szovegir("TOVÁBB", feher, font_h, renderer, teglalap.w, teglalap.x, teglalap.y);
    SDL_RenderPresent(renderer);

    SDL_Event ev;
    bool sikereskatt=false;
    while(!sikereskatt){
        SDL_WaitEvent(&ev);
        int x, y;
        switch(ev.type){
        case SDL_QUIT:
            SDL_Quit();
            return 1;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<= teglalap.x+teglalap.w && x >= teglalap.x && y>=teglalap.y && y <= teglalap.y+teglalap.h && ev.button.button==SDL_BUTTON_LEFT){
                sikereskatt=true;}
        }
    }

    TTF_CloseFont(font_h);
    return 0;
}