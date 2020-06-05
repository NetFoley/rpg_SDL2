#ifndef CHEST_H_INCLUDED
#define CHEST_H_INCLUDED

#include "sprite.h"
#include "gameObject.h"

typedef struct object{
    sprite sprIdle;
    gameObject gameObject;

}object;

void object_INI(object * o, SDL_Renderer * renderer, int t, int x, int y);
void object_MOVETO(struct object * o, int posX, int posY);
void object_DRAW(object * o,  SDL_Renderer * renderer, int offsetX, int offsetY);

void object_INI(object * o, SDL_Renderer * renderer, int t, int x, int y)
{

    char tObjectI[50];
    switch (t)
    {
        case 0 :
            gameObject_INI(&o->gameObject, 100, x, y, 0, 0, 0, renderer);
            strcpy(tObjectI, "wall.png");
            sprite_INI(&o->sprIdle, tObjectI, 0, -9, 16, 1, renderer);
            o->gameObject.boxCollider = o->sprIdle.imgPos;

        break;
        case 1 :
            gameObject_INI(&o->gameObject, 100, x, y, 0, 0, 0, renderer);
            strcpy(tObjectI, "chest.png");
            sprite_INI(&o->sprIdle, tObjectI, 4, 1, 16, 3, renderer);
            o->gameObject.boxCollider = o->sprIdle.imgPos;
        break;
    }
}

void object_MOVETO(struct object * o, int posX, int posY)
{
    o->gameObject.X = posX;
    o->gameObject.Y = posY;
}

void object_DRAW(object * o,  SDL_Renderer * renderer, int offsetX, int offsetY)
{
    if(sprite_DRAW(&o->sprIdle,  o->gameObject.timeSinceLastFrame, renderer, o->gameObject.X + offsetX, o->gameObject.Y + offsetY))
        o->gameObject.timeSinceLastFrame = SDL_GetTicks();
}

#endif // CHEST_H_INCLUDED
