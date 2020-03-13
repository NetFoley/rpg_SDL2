#include "SDL.h"
#include "SDL_image.h"
#include "layer.h"

int main(int argc, char *argv[])
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Event event;

    layer Layer1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize Image: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    char sMap[50];
    strcpy(sMap, "map1.map");
    layer_INI(&Layer1, renderer);
    layer_readMap(&Layer1, sMap, renderer);
    printf("Chargement de la carte %s\n", sMap);
    printf("Creation du layer : \nid du joueur:%i\nNombre de perso : %i\nNombre d'objects : %i\n", Layer1.idPlayer,Layer1.nbPerso, Layer1.nbObject);

    while (1) {

        while(SDL_PollEvent(&event))
        {
            layer_getInput(&Layer1, event);
        }

        layer_getInput(&Layer1, event);
        if (event.type == SDL_QUIT) {
            break;
        }

        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0x10, 0x65, 0x25, 0x00);
        SDL_RenderClear(renderer);
        layer_DRAW(&Layer1, renderer, window);
        SDL_RenderPresent(renderer);
    }

    printf("\nFin du programme\n");
    printf("\nPersonnage : \nPV : %i \nx : %f\ny : %f\nNombre de perso : %i\nNombre d'objects : %i", Layer1.player->gameObject.life, Layer1.player->gameObject.X, Layer1.player->gameObject.Y, Layer1.nbPerso, Layer1.nbObject);
    layer_Destroy(&Layer1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
