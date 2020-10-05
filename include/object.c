#include "object.h"
void object_INI(object * o, object * model, int x, int y)
{
    *o = *model;
    gameObject_INI(&o->gameObject, model->gameObject.name, 10, 0, x, y, 0);
}

void modelObject_INI(object * o, char * name, sprite sprites[1])
{
    gameObject_INI(&o->gameObject, name, 10, 0, 0, 0, 0);
    o->sprIdle = sprites[0];
    o->gameObject.boxCollider = o->sprIdle.imgPos;
}

void object_MOVETO(struct object * o, int posX, int posY)
{
    o->gameObject.coord.x = posX;
    o->gameObject.coord.x = posY;
}

void object_DRAW(object * o,  SDL_Renderer * renderer, int offsetX, int offsetY)
{
    sprite_DRAW(&o->sprIdle,  renderer, o->gameObject.coord.x + offsetX, o->gameObject.coord.y + offsetY);
}
