#include "SDL.h"
#include "SDL_image.h"
#include "include/scene.h"

int main(int argc, char *argv[])
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Event event;

    scene scene1;

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

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    char sMap[255];
    strcpy(sMap, MAPPATH);
    strcat(sMap, "stress2.map");
    scene_INI(&scene1, renderer);
    scene_readMap(&scene1, sMap);


    printf("Loading of map : %s\n", sMap);
    printf("Creation of scene : \nid of player:%i\nNumber of characters: %i\nNumber of objects : %i\n", scene1.idPlayer,scene1.nbPerso, scene1.nbObject);
    printf("\n");

    while (1) {

        while(SDL_PollEvent(&event))
        {
            scene_getInput(&scene1, event);
        }

        scene_getInput(&scene1, event);
        if (event.type == SDL_QUIT) {
            break;
        }

        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0x10, 0x65, 0x25, 0x00);
        SDL_RenderClear(renderer);
        scene_DRAW(&scene1, renderer, window);
        SDL_RenderPresent(renderer);
    }

    printf("End of program\n");
    printf("\nPlayer : \nLife : %i/%i \nx : %f\ny : %f\nNumber of characters: %i\nNumber of objects : %i\n", scene1.player->gameObject.life, scene1.player->gameObject.maxLife, scene1.player->gameObject.coord.x, scene1.player->gameObject.coord.y, scene1.nbPerso, scene1.nbObject);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
