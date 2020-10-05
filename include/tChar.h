#ifndef TCHAR_H_INCLUDED
#define TCHAR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "pile.h"
#include "object.h"

typedef struct tChar{
    vect2D pos;
    char ch;
}tChar;

typedef struct pathMap{
    int lenght;
    object * objects;
    int nbObject;
    vect2D offset;
    tChar * tchars;
}pathMap;


void iniPathMap(pathMap * pm, int offsetX, int offsetY, object * objects, int nbObject);
int settChar(pathMap * pm, int x, int y, char ch);
char map_readMap(pathMap * pm, int x, int y);
char gettCharPathMap(pathMap * pm, int x, int y);
int findtChar(pathMap pm, int x, int y);
SDL_bool validPmCoord(pathMap pm, int x, int y);
int getSizePathMap(pathMap pm);


#endif // TCHAR_H_INCLUDED
