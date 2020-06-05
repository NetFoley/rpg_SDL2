#ifndef BOXDRAWER_H_INCLUDED
#define BOXDRAWER_H_INCLUDED

#include "SDL.h"

void draw_Box(SDL_Rect b, SDL_Color, SDL_Renderer * renderer);

void draw_Box(SDL_Rect b, SDL_Color c, SDL_Renderer * renderer)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = SDL_CreateRGBSurface(0, b.w, b.h, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, c.r, c.g, c.b, c.a));
    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }


    SDL_RenderCopy(renderer, texture, NULL, &b);

}

#endif // BOXDRAWER_H_INCLUDED
