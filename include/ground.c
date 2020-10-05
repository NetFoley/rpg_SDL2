#include "ground.h"
void groundTile_INI(groundTile * o, groundTile * model, groundState state, float growth, int x, int y)
{
    *o = *model;
    o->growth = growth;
    o->coord.x = x;
    o->coord.y = y;
    if(state == herb)
    {
        if(rand()%5 == 0)
            o->groundState = flower;
    }
    else
        o->groundState = state;
}

void modelGroundTile_INI(groundTile * o, char * name, sprite sprites[field+1])
{
    strcpy(o->name, name);
    o->growth = 0;
    o->coord.x = 0;
    o->coord.y = 0;
    o->groundState = herb;
    for(int i = 0; i < field+1; i++)
        o->sprites[i] = sprites[i];
}

void groundTile_MOVETO(groundTile * o, int posX, int posY)
{
    o->coord.x = posX;
    o->coord.x = posY;
}

void groundTile_DRAW(groundTile * o,  SDL_Renderer * renderer, int offsetX, int offsetY)
{
    if(sprite_DRAW(&o->sprites[o->groundState], renderer, o->coord.x + offsetX, o->coord.y + offsetY))
    {
        o->timeSinceLastFrame = SDL_GetTicks();
        if(groundTile_IsField(*o))
            groundTile_addGrowth(o, (rand()%10+1)/5);
        if(o->groundState == seed && o->growth >= 100)
        {
            o->groundState = field;
        }
    }
}

void groundTile_addGrowth(groundTile * gT, float fGrowth)
{
    if(gT != NULL && groundTile_IsField(*gT))
    {
        if(gT->growth + fGrowth < 100)
        {
            gT->growth += fGrowth;
        }
        else
        {
            gT->growth = 100;
        }
    }
}

groundTile * groundTile_getTileFromPos(groundTile * groundTiles, int nbGroundTile, int x, int y)
{
    groundTile * gt = NULL;
    for(int i = 0; i < nbGroundTile; i ++)
    {
        if(groundTiles[i].coord.x / TILESIZE == x && groundTiles[i].coord.y /TILESIZE == y)
            gt = &groundTiles[i];
    }
    return gt;
}

SDL_bool groundTile_IsField(groundTile gt)
{
    SDL_bool isField = SDL_FALSE;
    if(gt.groundState == seed || gt.groundState == field)
    {
        isField = SDL_TRUE;
    }
    return isField;
}

void groundTile_changeState(groundTile * gt, groundState groundState)
{
    if(gt != NULL)
    {
        gt->groundState = groundState;
        if(groundTile_IsField(*gt))
        {
            gt->growth = 0;
        }
    }
    else
        printf("undefined groundTile\n");
}
