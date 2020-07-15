#ifndef BOXDRAWER_H_INCLUDED
#define BOXDRAWER_H_INCLUDED

#include "SDL.h"

SDL_Texture * ini_Box(int w, int h, int r, int g, int b, SDL_Renderer * renderer);

SDL_Texture * ini_Box(int w, int h, int r, int g, int b, SDL_Renderer * renderer)
{

    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, 0));
    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }

    SDL_SetTextureAlphaMod(texture, 180);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

#endif // BOXDRAWER_H_INCLUDED
