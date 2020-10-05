#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"

void sprite_INI(sprite * leSprite, char Anim[255], int offsetX, int offsetY, int tailleX, SDL_Renderer * renderer)
{
    strcpy(leSprite->cAnim, IMAGEPATH);
    strcat(leSprite->cAnim, Anim);
    //printf("Initialisation sprite : %s\n", leSprite->cAnim);

    leSprite->frame = 0;
    leSprite->scale = 3;
    leSprite->repeat = 1;
    leSprite->flip = SDL_FLIP_NONE;

    leSprite->offsetX = offsetX * leSprite->scale;
    leSprite->offsetY = offsetY * leSprite->scale;

    leSprite->imgSize.x = 0;
    leSprite->imgSize.y = 0;
    leSprite->imgSize.w = tailleX;

    leSprite->imgPos.x = 0;
    leSprite->imgPos.y = 0;
    leSprite->imgPos.w = leSprite->imgSize.w * leSprite->scale;

    leSprite->timeSinceLastFrame = 0;
    //Creation de la texture
    leSprite->surface = IMG_Load(leSprite->cAnim);
    if (!leSprite->surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    }

    leSprite->texture = SDL_CreateTextureFromSurface(renderer, leSprite->surface);
    if (!leSprite->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }

    leSprite->tailleImage = leSprite->surface->w;
    leSprite->imgSize.h = leSprite->surface->h;
    leSprite->imgPos.h = leSprite->imgSize.h * leSprite->scale;
    sprite_setTotalTime(leSprite, 100);

}

int sprite_DRAW(sprite * leSprite, SDL_Renderer * renderer, int X, int Y)
{
    if(leSprite == NULL)
        exit(EXIT_FAILURE);
    leSprite->imgPos.x = X + leSprite->offsetX - leSprite->imgPos.w/2 + 24;
    leSprite->imgPos.y = Y + leSprite->offsetY - leSprite->imgPos.h + 48;
    int draw = 0;

    if(SDL_GetTicks() - leSprite->timeSinceLastFrame >= sprite_getTickInBetween(*leSprite))
    {

        if(leSprite->imgSize.w * (leSprite->frame+1) < leSprite->tailleImage)
        {
            leSprite->frame++;
            draw = 1;
            leSprite->timeSinceLastFrame = SDL_GetTicks();
        }
        else if(leSprite->repeat == 1)
        {

            leSprite->frame = 0;
            draw = 1;
            leSprite->timeSinceLastFrame = SDL_GetTicks();
        }
    }


    leSprite->imgSize.x = leSprite->imgSize.w * leSprite->frame;

    SDL_RenderCopyEx(renderer, leSprite->texture, &leSprite->imgSize, &leSprite->imgPos, 0, NULL, leSprite->flip);
    return draw;
}


int sprite_animationEnded(sprite * spr)
{
    if(spr->repeat == SDL_TRUE
    || (spr->repeat == SDL_FALSE
        && spr->imgSize.w * (spr->frame+1) >= spr->tailleImage
        && sprite_getTickInBetween(*spr) + spr->timeSinceLastFrame < SDL_GetTicks()))
    {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

void sprite_RESET(sprite * leSprite)
{
    leSprite->timeSinceLastFrame = SDL_GetTicks();
    leSprite->frame = 0;
}

void sprite_setTotalTime(sprite * s, int t)
{
    s->totalFrameTime = t;
}

int sprite_getTickInBetween(sprite s)
{
    return s.totalFrameTime / sprite_getNumberOfFrame(s);
}

int sprite_getNumberOfFrame(sprite s)
{
    return (s.tailleImage/s.imgSize.w);
}

void sprite_FLIP_LEFT(sprite * spr)
{
    if (spr->flip != SDL_FLIP_HORIZONTAL)
    {
        spr->offsetX = -spr->offsetX;
        spr->flip = SDL_FLIP_HORIZONTAL;
    }
}

void sprite_FLIP_RIGHT(sprite * spr)
{
    if (spr->flip != SDL_FLIP_NONE)
    {
        spr->offsetX = -spr->offsetX;
        spr->flip = SDL_FLIP_NONE;
    }
}

int sprite_cmp(sprite spr1, sprite spr2)
{
    return strcmp(spr1.cAnim, spr2.cAnim);
}

int sprite_getFrame(sprite spr)
{
    return spr.frame;
}

int sprite_getWidth(sprite spr)
{
    return spr.imgPos.w;
}

int sprite_getHeight(sprite spr)
{
    return spr.imgPos.h;
}
