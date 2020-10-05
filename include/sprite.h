#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#define IMAGEPATH "images/"


typedef struct sprite{
    char cAnim[255];
    SDL_Rect imgSize;
    SDL_Rect imgPos;
    SDL_Texture * texture;
    SDL_Surface * surface;
    int tailleImage;
    int offsetX;
    int offsetY;
    int repeat;
    int frame;
    int totalFrameTime;
    Uint32 timeSinceLastFrame;
    int scale;
    SDL_RendererFlip flip;
}sprite;

void sprite_INI(sprite * leSprite, char Anim[255], int offsetX, int offsetY, int tailleX, SDL_Renderer * renderer);
int sprite_DRAW(sprite * leSprite, SDL_Renderer * renderer, int X, int Y);
void sprite_RESET(sprite * leSprite);
void sprite_FLIP_LEFT(sprite * spr);
void sprite_FLIP_RIGHT(sprite * spr);
int sprite_cmp(sprite spr1, sprite spr2);
int sprite_getFrame(sprite spr);
int sprite_getWidth(sprite spr);
int sprite_getHeight(sprite spr);
int sprite_animationEnded(sprite * spr);
void sprite_setTotalTime(sprite * s, int t);
int sprite_getTickInBetween(sprite s);
int sprite_getNumberOfFrame(sprite s);


#endif // SPRITE_H_INCLUDED
