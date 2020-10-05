#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED
#include "pile.h"

#include <stdio.h>
#include <stdlib.h>
#define MAPPATH "maps/"
#define PI 3.14159265

#include "object.h"
#include "tChar.h"
#include "pile.h"
#include "math.h"

typedef struct frontier{
    vect2D * coord;
    int taille;
}frontier;

int map_getPath(object * objects, int nbObject, int xS, int yS, int xE, int yE, file * path);
void map_putFrontier(pathMap * cPathMap, frontier * frontier, int x, int y);
vect2D map_getFrontier(frontier * f);
float getRadAngle(float pos1, float pos2);
vect2D map_getClosestFrontier(frontier * frt, vect2D pos);
direction getAngleDirection(float angle);
fvect2D getPosRatioFromAngle(float angle);
vect2D findPosFromAngleDist(int angle, int dist);

void freePathMap(pathMap * pm);
/*
void addXPathMap(pathMap * pm);
void addYPathMap(pathMap * pm);
*/

int getMapHeight(char sMap[255]);
int getMapWidth(char sMap[255]);

void iniFrontier(frontier * frt);
void freeFrontier(frontier * frt);


#endif // IA_H_INCLUDED
