#ifndef CHEST_H_INCLUDED
#define CHEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "gameObject.h"

#define OBJECTPATH "objects/"

typedef struct object{
    sprite sprIdle;
    gameObject gameObject;

}object;

void object_INI(object * o, object * model, int x, int y);
void modelObject_INI(object * o, char * name, sprite sprites[1]);
void object_MOVETO(struct object * o, int posX, int posY);
void object_DRAW(object * o,  SDL_Renderer * renderer, int offsetX, int offsetY);


#endif // CHEST_H_INCLUDED
