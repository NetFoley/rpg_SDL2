#include "interface.h"

void interface_INI(interface * i, int * offsetX, int * offsetY, SDL_Renderer * renderer)
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
    cursor_ini(&i->cursor, offsetX, offsetY, renderer);

}

SDL_Rect * interface_getOptionBox(interface * interface, int j)
{
    return &interface->options[j].pos;
}

void interface_addOption(interface * i, char *content, int (* func)())
{
    i->options =(option*)realloc(i->options, (i->nbOptions+1)*(sizeof(option)));
    TTF_Font * police = i->policeTiny;
    if(i->nbOptions == 0)
        police = i->policeMedium;
    option_ini(&i->options[i->nbOptions], police, content, i->renderer, func);
    i->nbOptions++;
}


void option_ini(option * op, TTF_Font * police, char * content, SDL_Renderer * renderer, int (*func)(perso*, perso*))
{
    op->nbText = 0;
    op->pos.x = 0;
    op->pos.y = 0;
    op->pos.w = 0;
    op->pos.h = 0;
    op->texts = (text*)malloc(sizeof(text));
    op->func = func;
    option_write(op, police, content, renderer);
}

void option_emptyText(option * op)
{
    free(op->texts);
    op->nbText = 0;
    op->texts = (text*)malloc(sizeof(text));
}

void option_addText(option * op, TTF_Font * police, char * content, SDL_Texture * texture)
{
    op->texts =(text*)realloc(op->texts, (op->nbText+1)*(sizeof(text)));
    text_ini(&op->texts[op->nbText], police, content, texture);
    op->nbText++;
}

void option_draw(option * o, TTF_Font * police, SDL_Renderer * renderer, SDL_bool selection)
{
    if(selection)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 235);
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 110);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &o->pos);
    for(int i = 0; i < o->nbText; i++)
    {
        text_draw(&o->texts[i], police, renderer, selection);
    }
}

void interface_desactivateAll(interface * interface)
{
    cursor_desactivate(&interface->cursor);
    interface_cacherOptions(interface);
}

void interface_cacherOptions(interface * i)
{
    i->optionActive = SDL_FALSE;
}

void interface_activerOption(interface * interface, perso * p)
{
    if(interface->options[interface->selection].func != NULL)
        interface->options[interface->selection].func (interface->persoSelected, p);
    interface_desactivateAll(interface);
}

void interface_afficherOptions(interface * interface, perso * p, int x, int y)
{
    interface_desactivateAll(interface);
    interface->optionActive = SDL_TRUE;
    interface->persoSelected = p;
    option_write(&interface->options[0], interface->policeMedium, p->gameObject.name, interface->renderer);
    option_move(&interface->options[0], p->gameObject.coord.x + x, p->gameObject.coord.y + y);

}

void interface_draw(interface * interface)
{
    if(interface->optionActive)
    {
        int wMax = 0;
        for(int i = 0; i < interface->nbOptions; i++)
        {
            if(interface->options[i].pos.w > wMax)
            {
                wMax = interface->options[i].pos.w;
            }
        }
        for(int i = 0; i < interface->nbOptions; i++)
        {
            interface->options[i].pos.w = wMax;
            if(i > 0)
                option_move(&interface->options[i], interface->options[i-1].pos.x, interface->options[i-1].pos.y + interface->options[i-1].pos.h);

            if(i == interface->selection || i == 0)
                option_draw(&interface->options[i], interface->policeTiny, interface->renderer, 1);
            else
                option_draw(&interface->options[i], interface->policeTiny, interface->renderer, 0);
        }

    }
    if(interface->cursor.active)
    {
        cursor_DRAW(&interface->cursor, interface->renderer);
    }
}

void text_draw(text * t, TTF_Font * police, SDL_Renderer * renderer, SDL_bool selection)
{
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

void cursor_ini(cursor * c, int * offsetX, int * offsetY, SDL_Renderer * renderer)
{
    c->focusTile = NULL;
    c->offsetX = offsetX;
    c->offsetY = offsetY;
    c->active = SDL_FALSE;

    char tCursor[255];
    strcpy(tCursor, "cursor.png");
    sprite_INI(&c->sprite, tCursor, 0, 2, 20, renderer);
    sprite_setTotalTime(&c->sprite, 300);
}

void interface_activateCursor(interface * interface)
{
    interface_desactivateAll(interface);
    cursor_activate(&interface->cursor);
}


void cursor_activate(cursor * c)
{
    c->active = SDL_TRUE;
}

groundTile * cursor_desactivate(cursor * c)
{
    c->active = SDL_FALSE;
    return cursor_getFocusTile(*c);
}

groundTile * cursor_getFocusTile(cursor c)
{
    return c.focusTile;
}

void cursor_DRAW(cursor * c, SDL_Renderer * renderer)
{
    if(c->focusTile != NULL)
        sprite_DRAW(&c->sprite, renderer, c->focusTile->coord.x + *c->offsetX, c->focusTile->coord.y + *c->offsetY);
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
    option_emptyText(op);
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
