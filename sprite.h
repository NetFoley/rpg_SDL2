#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <stdio.h>

typedef struct sprite{
    char cAnim[50];
    SDL_Rect imgSize;
    SDL_Rect imgPos;
    SDL_Texture * texture;
    SDL_Surface * surface;
    int tailleImage;
    int offsetX;
    int offsetY;
    int alive;
    int frame;
    int tickBetweenFrames;
    int scale;
    SDL_RendererFlip flip;
}sprite;

void sprite_INI(sprite * leSprite, char cAnim[50], int offsetX, int offsetY, int tailleX, int scale, SDL_Renderer * renderer);
int sprite_DRAW(sprite * leSprite, Uint32 timeSinceLastFrame, SDL_Renderer * renderer, int X, int Y);
int sprite_FDRAW(sprite * leSprite, Uint32 timeSinceLastFrame, SDL_Renderer * renderer, int X, int Y);
void sprite_RESET(sprite * leSprite);
void sprite_FLIP_LEFT(sprite * spr);
void sprite_FLIP_RIGHT(sprite * spr);
int sprite_cmp(sprite spr1, sprite spr2);
int sprite_getFrame(sprite spr);
int sprite_getWidth(sprite spr);
int sprite_getHeight(sprite spr);
int sprite_animationEnded(sprite spr);

void sprite_INI(sprite * leSprite, char Anim[50], int offsetX, int offsetY, int tailleX, int scale, SDL_Renderer * renderer)
{
    strcpy(leSprite->cAnim, Anim);
    //printf("Initialisation sprite : %s\n", leSprite->cAnim);

    leSprite->frame = 0;
    leSprite->tickBetweenFrames = 80;
    leSprite->scale = 3 * scale;
    leSprite->alive = 1;
    leSprite->flip = SDL_FLIP_NONE;

    leSprite->offsetX = offsetX * leSprite->scale;
    leSprite->offsetY = offsetY * leSprite->scale;

    leSprite->imgSize.x = 0;
    leSprite->imgSize.y = 0;
    leSprite->imgSize.w = tailleX;

    leSprite->imgPos.x = 0;
    leSprite->imgPos.y = 0;
    leSprite->imgPos.w = leSprite->imgSize.w * leSprite->scale;

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
}

int sprite_DRAW(sprite * leSprite, Uint32 timeSinceLastFrame, SDL_Renderer * renderer, int X, int Y)
{
    if(leSprite == NULL)
        exit(EXIT_FAILURE);
    leSprite->imgPos.x = X + leSprite->offsetX - leSprite->imgPos.w/2;
    leSprite->imgPos.y = Y + leSprite->offsetY - leSprite->imgPos.h/2;
    int draw = 0;

    if(leSprite->imgSize.w * (leSprite->frame+1) < leSprite->tailleImage)
    {
        if(SDL_GetTicks() - timeSinceLastFrame >= leSprite->tickBetweenFrames)
        {
            leSprite->frame++;
            draw = 1;
        }
    }
    else
    {
        if(SDL_GetTicks() - timeSinceLastFrame >= leSprite->tickBetweenFrames)
        {
            leSprite->frame = 0;
            draw = 1;
        }
    }


    leSprite->imgSize.x = leSprite->imgSize.w * leSprite->frame;

    SDL_RenderCopyEx(renderer, leSprite->texture, &leSprite->imgSize, &leSprite->imgPos, 0, NULL, leSprite->flip);
    return draw;
}

int sprite_FDRAW(sprite * leSprite, Uint32 timeSinceLastFrame, SDL_Renderer * renderer, int X, int Y)
{
    int draw = 0;
    leSprite->imgPos.x = X + leSprite->offsetX - leSprite->imgPos.w/2;
    leSprite->imgPos.y = Y + leSprite->offsetY - leSprite->imgPos.h/2;

    if(leSprite->imgSize.w * (leSprite->frame+1) < leSprite->tailleImage && leSprite->alive == 1)
    {
        if(SDL_GetTicks() - timeSinceLastFrame >= leSprite->tickBetweenFrames)
        {
            leSprite->frame++;
            draw = 1;
        }
    }
    else if(leSprite->alive == 1)
    {
        leSprite->frame--;
        leSprite->alive = 0;
        draw = 1;
    }


    leSprite->imgSize.x = leSprite->imgSize.w * leSprite->frame;

    SDL_RenderCopyEx(renderer, leSprite->texture, &leSprite->imgSize, &leSprite->imgPos, 0, NULL, leSprite->flip);

    return draw;
}

int sprite_animationEnded(sprite spr)
{
    if(spr.imgSize.w * (spr.frame+1) < spr.tailleImage)
        return 0;
    else
        return 1;
}

void sprite_RESET(sprite * leSprite)
{
    leSprite->frame = 0;
}

void sprite_setTickInBetween(sprite * s, int t)
{
    s->tickBetweenFrames = t;
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

#endif // SPRITE_H_INCLUDED
