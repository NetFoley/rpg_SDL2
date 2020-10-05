#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "SDL_ttf.h"
#include "string.h"
#include "perso.h"
#include "sprite.h"
#include "ground.h"

#include <stdio.h>
#include <stdlib.h>
#define MAXOPTIONLETTERS 255
#define MAXOPTIONLENGHT 200


#define TINY 18
#define MEDIUM 23
#define BIG 35

typedef struct cursor{
    groundTile * focusTile;
    int * offsetX;
    int * offsetY;
    sprite sprite;
    SDL_bool active;
}cursor;

typedef struct text{
    char content[MAXOPTIONLETTERS];
    SDL_Rect pos;
    SDL_Texture * texture;

}text;

typedef struct option{
    text * texts;
    int nbText;
    SDL_Rect pos;
    int (*func)();
}option;

typedef struct interface{
    cursor cursor;
    TTF_Font * policeTiny;
    TTF_Font * policeMedium;
    TTF_Font * policeBig;
    perso * persoSelected;
    SDL_Renderer * renderer;

    option * options;
    SDL_bool optionActive;
    int nbOptions;
    int selection;
}interface;

void interface_INI(interface * i, int * offsetX, int * offsetY, SDL_Renderer * renderer);
SDL_Rect * interface_getOptionBox(interface * interface, int j);
void interface_activerOption(interface * interface, perso * p);
void interface_cacherOptions(interface * i);
void interface_addOption(interface * i, char *content, int (* func)());
void interface_afficherOptions(interface * interface, perso * p, int x, int y);
void interface_activateCursor(interface * interface);
void interface_draw(interface * interface);

void text_ini(text * t, TTF_Font * police, char *content, SDL_Texture * texture);
void text_draw(text * t, TTF_Font * police, SDL_Renderer * renderer, SDL_bool selection);
void text_move(text * t, int x, int y);

void option_ini(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer, int (*func)(perso*, perso*));
void option_write(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer);
void option_move(option * o, int x, int y);
void option_emptyText(option * op);

void cursor_ini(cursor * c, int * offsetX, int * offsetY, SDL_Renderer * renderer);
groundTile * cursor_getFocusTile(cursor c);
void cursor_activate(cursor * c);
groundTile * cursor_desactivate(cursor * c);
void cursor_DRAW(cursor * c, SDL_Renderer * renderer);

#endif // INTERFACE_H_INCLUDED
