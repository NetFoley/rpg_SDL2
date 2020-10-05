#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "gameObject.h"
#include "sprite.h"

#define GROUNDPATH "groundTiles/"

typedef enum groundState{herb, flower, dirt, seed, field}groundState; //KEEP field THE LAST ONE

typedef struct groundTile{
    char name[255];
    groundState groundState;
    float growth;
    fvect2D coord;
    sprite sprites[field + 1];
    Uint32 timeSinceLastFrame;
}groundTile;

void groundTile_INI(groundTile * o, groundTile * model, groundState state, float growth, int x, int y);
void modelGroundTile_INI(groundTile * o, char * name, sprite sprites[field+1]);
void groundTile_MOVETO(groundTile * o, int posX, int posY);
void groundTile_DRAW(groundTile * o,  SDL_Renderer * renderer, int offsetX, int offsetY);
void groundTile_addGrowth(groundTile * gT, float fGrowth);
groundTile * groundTile_getTileFromPos(groundTile * groundTiles, int nbGroundTile, int x, int y);
void groundTile_changeState(groundTile * gt, groundState groundState);
SDL_bool groundTile_IsField(groundTile gt);

#endif // GROUND_H_INCLUDED
