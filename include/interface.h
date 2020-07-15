#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "SDL_ttf.h"
#include "string.h"

#define MAXOPTIONLETTERS 255
#define MAXOPTIONLENGHT 200


#define TINY 20
#define MEDIUM 40
#define BIG 60

typedef struct text{
    char content[MAXOPTIONLETTERS];
    SDL_Rect pos;
    SDL_Texture * texture;

}text;

typedef struct option{
    text * texts;
    int nbText;
    SDL_Rect pos;
}option;

typedef struct interface{
    TTF_Font * policeTiny;
    TTF_Font * policeMedium;
    TTF_Font * policeBig;
    option * options;
    SDL_bool optionActive;
    int nbOptions;
    int selection;
    SDL_Renderer * renderer;
}interface;

void text_ini(text * t, TTF_Font * police, char *content, SDL_Texture * texture);
void text_draw(text * t, TTF_Font * police, SDL_Renderer * renderer, SDL_bool selection);
void interface_addOption(interface * i, char *content);
void interface_afficherOptions(interface * i, perso p, int x, int y);
void interface_draw(interface * interface);
void text_move(text * t, int x, int y);
void option_ini(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer);
void option_write(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer);
void option_move(option * o, int x, int y);

void interface_INI(interface * i, SDL_Renderer * renderer)
{
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    i->policeTiny = TTF_OpenFont("font/COOPBL.ttf", TINY);
    i->policeMedium = TTF_OpenFont("font/COOPBL.ttf", MEDIUM);
    i->policeBig = TTF_OpenFont("font/COOPBL.ttf", BIG);
    i->options = (option*)malloc(sizeof(option));
    i->optionActive = SDL_FALSE;
    i->nbOptions = 0;
    i->selection = 0;
    i->renderer = renderer;
    interface_addOption(i, "name");
    interface_addOption(i, "Command");
    interface_addOption(i, "placeholder 1");
    interface_addOption(i, "placeholder 2");
    interface_addOption(i, "ET salut à tous les amis c'est david la farge pokemon j'espère que vous allez bien on se retrouve aujourd'hui pour un unboxing");
    interface_addOption(i, "Kill");

}

SDL_Rect * interface_getOptionBox(interface * interface, int j)
{
    return &interface->options[j].pos;
}

void interface_addOption(interface * i, char *content)
{
    i->options =(option*)realloc(i->options, (i->nbOptions+1)*(sizeof(option)));
    option_ini(&i->options[i->nbOptions], i->policeTiny, content, i->renderer);
    i->nbOptions++;
}

void option_ini(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer)
{
    op->nbText = 0;
    op->pos.x = 0;
    op->pos.y = 0;
    op->pos.w = 0;
    op->pos.h = 0;
    op->texts = (text*)malloc(sizeof(text));
    option_write(op, police, content, renderer);
}

void option_addText(option * op, TTF_Font * police, char * content, SDL_Texture * texture)
{
    op->texts =(text*)realloc(op->texts, (op->nbText+1)*(sizeof(text)));
    text_ini(&op->texts[op->nbText], police, content, texture);
    op->nbText++;
}

void option_draw(option * o, TTF_Font * police, SDL_Renderer * renderer, SDL_bool selection)
{
    for(int i = 0; i < o->nbText; i++)
    {
        text_draw(&o->texts[i], police, renderer, selection);
    }
}

void interface_afficherOptions(interface * interface, perso p, int x, int y)
{
    interface->optionActive = SDL_TRUE;
    option_ini(&interface->options[0], interface->policeTiny, p.name, interface->renderer);
    option_move(&interface->options[0], p.gameObject.coord.x + x, p.gameObject.coord.y + y);

}

void interface_cacherOptions(interface * i)
{
    i->optionActive = SDL_FALSE;
}

void interface_draw(interface * interface)
{
    if(interface->optionActive)
        for(int i = 0; i < interface->nbOptions; i++)
        {
            if(i > 0)
                option_move(&interface->options[i], interface->options[i-1].pos.x, interface->options[i-1].pos.y + interface->options[i-1].pos.h);

            if(i == interface->selection)
                option_draw(&interface->options[i], interface->policeTiny, interface->renderer, 1);
            else
                option_draw(&interface->options[i], interface->policeTiny, interface->renderer, 0);
        }
}

void text_draw(text * t, TTF_Font * police, SDL_Renderer * renderer, SDL_bool selection)
{
    if(selection)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 235);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 110);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &t->pos);
    SDL_RenderCopy(renderer, t->texture, NULL, &t->pos);
}

/******************************
**************TEXT*************
******************************/
void text_ini(text * t, TTF_Font * police, char *content, SDL_Texture * texture)
{
    if(strlen(content) > 255)
        exit(EXIT_FAILURE);

    t->pos.x = 0;
    t->pos.y = 0;
    t->pos.w = 0;
    t->pos.h = 0;
    strcpy(t->content, content);
    TTF_SizeText(police, t->content, &t->pos.w, &t->pos.h);

    t->texture = texture;
}

void option_move(option * o, int x, int y)
{
    o->pos.x = x;
    o->pos.y = y;
    for(int i = 0 ; i < o->nbText; i++)
    {
        o->texts[i].pos.x = o->pos.x;
        if(i == 0)
            o->texts[i].pos.y = o->pos.y;
        else
            o->texts[i].pos.y = o->pos.y + i * o->texts[i-1].pos.h;
    }
}

void option_write(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer)
{
    SDL_Color couleurBlanche = {255, 255, 255};

    int comp = 0;
    int nCar = 0;
    while(nCar < strlen(content))
    {
        char str[MAXOPTIONLETTERS+1];
        strcpy(str, "");
        int w = 0,h = 0;
        do
        {
            strncat(str, &content[nCar],1);
            TTF_SizeText(police, str, &w, &h);
            nCar++;

        }
        while(w < MAXOPTIONLENGHT && content[nCar] != '\0');
        //strncpy(str, t->content, MAXOPTIONLETTERS);


        SDL_Surface * rowSurface = TTF_RenderText_Blended(police, str, couleurBlanche);
        if (rowSurface == NULL) {
            SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
            exit(1);
        }


        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, rowSurface);
        if (!texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        }
        option_addText(op, police, str, texture);

        comp++;
    }
    TTF_SizeText(police, op->texts[0].content, &op->pos.w, &op->pos.h);
    op->pos.h = op->pos.h * comp;
}
#endif // INTERFACE_H_INCLUDED
