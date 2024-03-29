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

#include "debugmalloc.h"

SDL_Color feher = {255, 255, 255};
SDL_Color fekete = {0, 0, 0};
SDL_Color piros = {255, 0, 0};
SDL_Color sarga = {255, 255, 0};

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

    const int w=800;
    const int h=600;

    SDL_Texture *menukep= IMG_LoadTexture(renderer, "menukep.jpg");
    if (menukep == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    //kirajzol
    SDL_Rect dest = { 0, 0, w, h };
    SDL_RenderCopy(renderer, menukep, NULL, &dest);

    SDL_DestroyTexture(menukep);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 60);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    rectangleRGBA(renderer, w/4, 200, 3*w/4, 300, 255, 255, 255, 255);
    szovegir("Új játék", feher, font, renderer, 800, 0, 200+20);
    rectangleRGBA(renderer, w/4, 325, 3*w/4, 425, 255, 255, 255, 255);
    szovegir("Ranglista", feher, font, renderer, 800, 0,  325+20);
    rectangleRGBA(renderer, w/4, 450, 3*w/4, 550, 255, 255, 255, 255);
    szovegir("Kilépés", feher, font, renderer, 800, 0, 450+20);

    font=TTF_OpenFont("LiberationSerif-Regular.ttf", 100);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    szovegir("AKNAKERESŐ", feher, font, renderer, w, 0, 50);

    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);

    SDL_Event ev;
    int x, y;
    while(1){
        SDL_WaitEvent(&ev);
        switch(ev.type){
        case SDL_QUIT:
            return 3;
        case SDL_MOUSEBUTTONDOWN:
            x=ev.button.x;
            y=ev.button.y;
            break;
        case SDL_MOUSEBUTTONUP:
            if(x<=3*w/4 && x >= w/4 && y>=200 && y <=300 && ev.button.button==SDL_BUTTON_LEFT)
                return 1;
            if(x<=3*w/4 && x >= w/4 && y>=325 && y <=425 && ev.button.button==SDL_BUTTON_LEFT)
                return 2;
            if(x<=3*w/4 && x >= w/4 && y>=450 && y <=550 && ev.button.button==SDL_BUTTON_LEFT)
                return 3;
            break;
            }
        }
}

int static palyaadatok_beolvas(SDL_Renderer *renderer, TTF_Font *font, int *adat, int felsokorl, int alsokorl, char *adatnev, char *hibasok, char *hibakeves){
    int beolv_allapot=beolvas_rajzol(renderer, adat, adatnev);

    if(beolv_allapot==1){
        TTF_CloseFont(font);
        return 1;
    }

    while(*adat > felsokorl || *adat < alsokorl || beolv_allapot==2){
        if(beolv_allapot==2) //ez felsőbbrendő, mert ha karakter mennek be bemenetként azt 0-ra váltódik, ezért kiírná a az alsókorlát alatti hibaüzenetet is
            szovegir("Nem lehet betű a bemenetben.", piros, font, renderer, 800, 0, 520);
        else{
            if(*adat < felsokorl) szovegir(hibakeves, piros, font, renderer, 800, 0, 520);
            if(*adat > alsokorl) szovegir(hibasok, piros, font, renderer, 800, 0, 520);
        }
        szovegir("Hibás bemenet.", piros, font, renderer, 800, 0, 450);
        beolv_allapot=beolvas_rajzol(renderer, adat, adatnev);
        if(beolv_allapot==1){
            TTF_CloseFont(font);
            return 1;
        }
    }

    return 0;
}

int almenu(SDL_Renderer *renderer, Palyaparameterek *parameterek){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", 60);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    rectangleRGBA(renderer, 200, 63, 600, 163, 255, 255, 255, 255);
    szovegir("Könnyű", feher, font, renderer, 800, 0, 63+20);
    rectangleRGBA(renderer, 200, 188, 600, 288, 255, 255, 255, 255);
    szovegir("Közepes", feher, font, renderer, 800, 0, 188+20);
    rectangleRGBA(renderer, 200, 312, 600, 412, 255, 255, 255, 255);
    szovegir("Nehéz", feher, font, renderer, 800, 0, 312+20);
    rectangleRGBA(renderer, 200, 437, 600, 537, 255, 255, 255, 255);
    szovegir("Egyéni", feher, font, renderer, 800, 0, 437+20);

    SDL_RenderPresent(renderer);

    SDL_Event ev;
    int x, y;
    bool sikereskatt=false;
    while(!sikereskatt){
        SDL_WaitEvent(&ev);
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
                parameterek->szelesseg=8;
                parameterek->magassag=8;
                parameterek->bombaszam=10;
                sikereskatt=true;}
            if(x<=600 && x >= 200 && y>=188 && y <= 288 && ev.button.button==SDL_BUTTON_LEFT){
                parameterek->szelesseg=16;
                parameterek->magassag=16;
                parameterek->bombaszam=40;
                sikereskatt=true;}
            if(x<=600 && x >= 200 && y>=312 && y <= 412 && ev.button.button==SDL_BUTTON_LEFT){
                parameterek->szelesseg=30;
                parameterek->magassag=16;
                parameterek->bombaszam=99;
                sikereskatt=true;}
            if(x<=600 && x >= 200 && y>=437 && y <= 537 && ev.button.button==SDL_BUTTON_LEFT){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);//az almenü nem törlődik ki, ezért kell
                SDL_DisplayMode DM;
                SDL_GetDesktopDisplayMode(0, &DM);

                //ha bármelyik 1 akkor a következők nem lesznek már megnézve
                if(palyaadatok_beolvas(renderer, font, &parameterek->szelesseg, DM.w/20, 6, "X", "Túl nagy a méret.", "Minimum 6 széles.")==1 ||
                   palyaadatok_beolvas(renderer, font, &parameterek->magassag, DM.h/20-4, 6, "Y", "Túl nagy a méret.", "Minimum 6 széles.")==1 || //-4, hogy az ablak fejléce kiférjen a képernyőre, illetve hogy a képernyő alján a taskbar alá ne lógjon be az ablak
                   palyaadatok_beolvas(renderer, font, &parameterek->bombaszam, (parameterek->szelesseg)*(parameterek->magassag)-1, 1, "Bombaszám", "Túl sok bomba.", "Minimum 1 bomba.")==1){
                    TTF_CloseFont(font);
                    return 1;
                }
                sikereskatt=true;
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

    for(int h=0; h<j.parameterek.magassag; h++){
        for(int w=0; w<j.parameterek.szelesseg; w++){
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
                    mezo_rajzol(renderer, mezokep, j.palya[h][w].nearbomb, w, h);//kihasználva hogy az első 9 kép a számokat tartalmazó kép
                break;
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(mezokep);
}

int jelolsdl(Jatek *j, Jeloles *aktjeloles){
    //jeloles
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

    //állapotváltoztatás
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

int nyert_rajzol(SDL_Renderer *renderer, SDL_Window *window, Eredmenyadatok *eredmeny){
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    int fontmeret;
    if(h < w)
        fontmeret=h/6;
    else
        fontmeret=w/6;

    SDL_Rect teglalap;
    teglalap.x=w/12;
    teglalap.y=h/2-fontmeret/2;
    teglalap.w=5*w/6;
    teglalap.h=fontmeret+3;

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", fontmeret);
    if(!font){
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Texture *konfetti= IMG_LoadTexture(renderer, "konfetti.png");
    if (konfetti == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    //kirajzol
    SDL_Rect dest = { 0, 0, w, h };
    SDL_RenderCopy(renderer, konfetti, NULL, &dest);

    szovegir("NYERTÉL", sarga, font, renderer, w, 0, h/8-fontmeret/2);
    szovegir("NÉV:", sarga, font, renderer, w, 0, teglalap.y-fontmeret);
    szovegir("IDŐ:", sarga, font, renderer, w, 0, teglalap.y+teglalap.h+h/6-fontmeret);

    char szoveg_ido[16];
    char idostr[5+1]; //egy napnál csak nem fut tovább a játék

    if(eredmeny->ido<=99999)
        strcpy(szoveg_ido, itoa(eredmeny->ido, idostr, 10));
    else
        strcpy(idostr, "99999"); //ha mégis megtörténne

    strcat(szoveg_ido, " másodperc");
    szovegir(szoveg_ido, sarga, font, renderer, w, 0, teglalap.y+teglalap.h+h/6);

    //név bekérés min 1 karakter
    if(!input_text(eredmeny->nev, 20, teglalap, fekete, sarga, font, renderer)){
        TTF_CloseFont(font);
        return 1;
    }
    while(strlen(eredmeny->nev)==0){
        TTF_Init();
        TTF_Font *font_hiba = TTF_OpenFont("LiberationSerif-Regular.ttf", 2*fontmeret/3);
        if(!font){
            SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
            exit(1);
        }
        szovegir("Min 1 karakter", piros, font_hiba, renderer, w, 0, teglalap.y+teglalap.h);
        if(!input_text(eredmeny->nev, 20, teglalap, fekete, sarga, font, renderer)){ //ha false akkor kilépett a felhasználó
            TTF_CloseFont(font);
            TTF_CloseFont(font_hiba);
            return 1;
        }
        TTF_CloseFont(font_hiba);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_DestroyTexture(konfetti);

    return tovabb_gomb(renderer, teglalap, font, fontmeret, sarga);
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
    szovegir("Ranglista", feher, font, renderer, w, 0, h/7+(2*h/7-2*h/10)/2);
    rectangleRGBA(renderer, w/10, 4*h/7, 9*w/10, 6*h/7, 255, 255, 255, 255);
    szovegir("Főmenü", feher, font, renderer, w, 0, 4*h/7+(2*h/7-2*h/10)/2);

    SDL_RenderPresent(renderer);
    TTF_CloseFont(font);

    SDL_Event ev;
    int x, y;
    while(1){
        SDL_WaitEvent(&ev);
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

    int fontmeret;
    if(h < w) //kiférjen mindenképp az ablakba a szöveg
        fontmeret=h/6;
    else
        fontmeret=w/6;

    //robbanaskep
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
    //TOVÁBB gomb
    SDL_Rect teglalap;
    teglalap.x=w/12;
    teglalap.y=h/2-fontmeret/2;
    teglalap.w=5*w/6;
    teglalap.h=fontmeret+3;

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("LiberationSerif-Regular.ttf", fontmeret);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_DestroyTexture(robbanas);

    return tovabb_gomb(renderer, teglalap, font, fontmeret, feher);
}

int ranglistakiir_sdl(ListaPalya *eleje, Palyaparameterek parameterek, SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const int w=800;
    const int h=600;

    TTF_Font *font_cim = TTF_OpenFont("LiberationSerif-Regular.ttf", h/7);
    if (!font_cim) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    szovegir("RANGLISTA", feher, font_cim, renderer, w, 0, 0);
    TTF_CloseFont(font_cim);

    TTF_Init();
    TTF_Font *font_h = TTF_OpenFont("LiberationSerif-Regular.ttf", h/14);
    if (!font_h) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    if(parameterek.szelesseg==8 && parameterek.magassag==8 && parameterek.bombaszam==10)
        szovegir("Könnyű", feher, font_h, renderer, w, 0, h/7);
    else if(parameterek.szelesseg==16 && parameterek.magassag==16 && parameterek.bombaszam==40)
        szovegir("Közepes", feher, font_h, renderer, w, 0, h/7);
    else if(parameterek.szelesseg==30 && parameterek.magassag==16 && parameterek.bombaszam==99)
        szovegir("Nehéz", feher, font_h, renderer, w, 0, h/7);
    else
        szovegir("Egyéb méretek", feher, font_h, renderer, w, 0, h/7);

    //eredmények kiírása
    if(eleje==NULL)
        szovegir("Még nincsenek eredmények.", feher, font_h, renderer, w, 0, h/2);
    else{
        int helyezes=0;
        ListaPalya *palyakeres=eleje;
        while(palyakeres->kovpalya!=NULL && !(palyakeres->palya_parameterek.szelesseg==parameterek.szelesseg && palyakeres->palya_parameterek.magassag==parameterek.magassag && palyakeres->palya_parameterek.bombaszam==parameterek.bombaszam))
            palyakeres=palyakeres->kovpalya;
            if(palyakeres->palya_parameterek.szelesseg==parameterek.szelesseg && palyakeres->palya_parameterek.magassag==parameterek.magassag && palyakeres->palya_parameterek.bombaszam==parameterek.bombaszam){
                for(ListaJatekosok *mozgo=palyakeres->jatekosok; mozgo!=NULL && helyezes<10; mozgo=mozgo->kov){
                    char helyezesstring[4];
                    itoa(helyezes+1, helyezesstring, 10);
                    strcat(helyezesstring, ".");
                    szovegir(helyezesstring, feher, font_h, renderer, 150, 0, (helyezes+3)*h/14);

                    char idostring[9];
                    itoa(mozgo->adatok.ido, idostring, 10);
                    strcat(idostring, " mp");
                    szovegir(idostring, feher, font_h,renderer, 250, 150, (helyezes+3)*h/14);

                    szovegir(mozgo->adatok.nev, feher, font_h,renderer, 400, 400, (helyezes+3)*h/14);
                    helyezes++;
                }
            }
            else szovegir("Még nincsenek eredmények.", feher, font_h, renderer, w, 0, h/2);
    }

    //TOVÁBB gomb
    SDL_Rect teglalap;
    teglalap.x=3*w/4;
    teglalap.y=h-h/14;
    teglalap.w=w/4;
    teglalap.h=h/14;

    return tovabb_gomb(renderer, teglalap, font_h, h/14, feher);
}
