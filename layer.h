#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include "perso.h"
#include "object.h"
#include "IA.h"

#define PI 3.14159265

typedef struct layer{
    int offsetX;
    int offsetY;
	perso * persos;
	object * objects;
	perso * player;
    int timeSinceLastAction;
    int timeBetweenAction;
	int nbPerso;
	int nbObject;
	int idPlayer;
}layer;

void layer_INI(layer * leLayer, SDL_Renderer * renderer);
void layer_DRAW(layer * leLayer, SDL_Renderer * renderer, SDL_Window * window);
void layer_AddSoldier(layer * leLayer, SDL_Renderer * renderer, int x, int y);
void layer_AddSquelette(layer * leLayer, SDL_Renderer * renderer, int x, int y);
void layer_AddCoffre(layer * leLayer, SDL_Renderer * renderer, int x, int y);
void layer_AddGrosCoffre(layer * leLayer, SDL_Renderer * renderer);
void layer_getInput(layer * l, SDL_Event event);
void layer_Destroy();
void layer_readMap(layer * l, char sMap[50], SDL_Renderer * renderer);
void layer_setPlayer(layer * lay, perso * p);
perso * layer_getPersoById(layer * lay, int id);

void layer_INI(layer * leLayer, SDL_Renderer * renderer)
{
    leLayer->offsetX = 0;
    leLayer->offsetY = 0;
    leLayer->persos =(perso*)malloc(2*sizeof(perso));
    leLayer->objects =(object*)malloc(2*sizeof(object));
	leLayer->nbPerso = 0;
	leLayer->nbObject = 0;
	leLayer->timeSinceLastAction = SDL_GetTicks();
	leLayer->timeBetweenAction = 30;
	leLayer->idPlayer = 0;
	leLayer->player = NULL;
}

void layer_AddSquelette(layer * leLayer, SDL_Renderer * renderer, int x, int y)
{
    leLayer->persos =(perso*)realloc(leLayer->persos, (leLayer->nbPerso+2)*(sizeof(perso)));
    perso_INI(&leLayer->persos[leLayer->nbPerso], renderer, 0, x, y);
    leLayer->nbPerso++;
    //printf("%i loaded perso\n\n", leLayer->nbPerso);
}

void layer_AddSoldier(layer * leLayer, SDL_Renderer * renderer, int x, int y)
{

    leLayer->persos =(perso*)realloc(leLayer->persos, (leLayer->nbPerso+2)*(sizeof(perso)));
    perso_INI(&leLayer->persos[leLayer->nbPerso], renderer, 1, x, y);
    leLayer->nbPerso++;
    //printf("%i loaded perso\n\n", leLayer->nbPerso);
}

void layer_AddCoffre(layer * leLayer, SDL_Renderer * renderer, int x, int y)
{
    //CA VA REBUGGER CORRIGER LA MEMOIRE AJOUTE AVEC LE BLOCK DE PATHFINDING
    leLayer->objects =(object*)realloc(leLayer->objects, (leLayer->nbObject+2)*(sizeof(object)));
    object_INI(&leLayer->objects[leLayer->nbObject], renderer, 0, x ,y);
    leLayer->nbObject++;
    //printf("%i loaded objects\n\n", leLayer->nbObject);
}

/*void layer_AddGrosCoffre(layer * leLayer, SDL_Renderer * renderer)
{
    if(leLayer->nbPerso < objetMAX)
    {
        object_INI(&leLayer->objects[leLayer->nbObject], renderer, 1);
        leLayer->nbObject++;
    }
}*/

void layer_readMap(layer * l, char sMap[50], SDL_Renderer * renderer)
{
    FILE * pFile;
    pFile = fopen(sMap, "r");
    if (pFile != NULL)
    {
        char a;
        int i = 0, j = 0;
        while(a != EOF)
        {
            a = fgetc(pFile);
            switch (a)
            {
            case 'c' :
                layer_AddCoffre(l, renderer, i*TILESIZE, j*TILESIZE);
                break;
            case 'P' :
                layer_AddSoldier(l, renderer, i*TILESIZE, j*TILESIZE);
                layer_setPlayer(l, &l->persos[l->nbPerso-1]);
                printf("Joueur id : %i\n", l->persos[l->nbPerso-1].gameObject.id);
                break;
            case '1' :
                layer_AddSoldier(l, renderer, i*TILESIZE, j*TILESIZE);
                break;
            case '2' :
                layer_AddSquelette(l, renderer, i*TILESIZE, j*TILESIZE);
                break;
            case '\n' :
                j++;
                i = -1;
                break;
            }
            i++;
        }
        fclose(pFile);
    }

}

void layer_setPlayer(layer * lay, perso * p)
{
    lay->idPlayer = p->gameObject.id;
    p->playerControled = SDL_TRUE;
}

perso * layer_getPersoById(layer * lay, int id)
{
    int persoId = 0;
    for(int i = 0; i < lay->nbPerso; i++)
    {
        if(lay->persos[i].gameObject.id == id)
        {
            persoId = i;
        }
    }
    return &lay->persos[persoId];
}

void layer_getInput(layer * l, SDL_Event event)
/**************************************
GESTION DES ENTREES CLAVIER
***************************************/
{
    if(l->player == NULL)
    {
        l->player = layer_getPersoById(l, l->idPlayer);
    }
    if(l->player->gameObject.id != l->idPlayer)
    {
        l->player = layer_getPersoById(l, l->idPlayer);
    }

    if(l->player->gameObject.life > 0 )
    {
        /*
        if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED) {
            moveToMouse(&l->persos[0].gameObject);
            gameObject_addLife(&l->persos[0].gameObject,10);
        }*/
        static SDL_bool pressed = SDL_FALSE;
        if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED && pressed == SDL_FALSE)
        {
            pressed = SDL_TRUE;
            SDL_Point mousePos;
            SDL_GetMouseState(&mousePos.x, &mousePos.y);

            mousePos.x -= l->offsetX - TILESIZE/2;
            mousePos.y -= l->offsetY - TILESIZE;

            printf("Mouse pos x %i y %i\n",mousePos.x, mousePos.y);
            for(int i = 0; i < l->nbPerso; i++)
            {

                if(SDL_PointInRect(&mousePos, gameObject_getBoxCollider(&l->persos[i].gameObject)))
                {
                    printf("CLICK ON OBJECT %i \n", i);
                }
            }
        }
        else if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED)
        {
            pressed = SDL_FALSE;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f) {
            perso_setAttack(l->player);
        }

            static SDL_bool Z=0,Q=0,S=0,D=0;

            switch(event.type){
                case SDL_KEYDOWN:
                //Déplacements au clavier
                if (event.key.keysym.sym == SDLK_d)
                {
                    D = 1;
                }
                if (event.key.keysym.sym == SDLK_q)
                {
                    Q = 1;
                }

                if (event.key.keysym.sym == SDLK_z )
                {
                    Z = 1;
                }
                if (event.key.keysym.sym == SDLK_s)
                {
                    S = 1;
                }
                break;

                case SDL_KEYUP:
                //Déplacements au clavier
                if (event.key.keysym.sym == SDLK_d)
                {
                    D = 0;
                }
                if (event.key.keysym.sym == SDLK_q)
                {
                    Q = 0;
                }

                if (event.key.keysym.sym == SDLK_z )
                {
                    Z = 0;
                }
                if (event.key.keysym.sym == SDLK_s)
                {
                    S = 0;
                }
                break;

            }

            if(Z && l->player->attacking != 1)
            {
                perso_moveUp(l->player);
            }
            else if(S && l->player->attacking != 1)
            {
                perso_moveDown(l->player);
            }
            else
            {
                perso_slowY(l->player);
            }
            if(Q && l->player->attacking != 1)
            {
                perso_moveLeft(l->player);
            }
            else if(D && l->player->attacking != 1)
            {
                perso_moveRight(l->player);
            }
            else
            {
                perso_slowX(l->player);
            }


    }
}

void layer_DRAW(layer * leLayer, SDL_Renderer * renderer, SDL_Window * window)
{
    Uint32 ticks = SDL_GetTicks();
    while(ticks - leLayer->timeSinceLastAction < leLayer->timeBetweenAction)
    {
        ticks = SDL_GetTicks();
    }
    leLayer->timeSinceLastAction += leLayer->timeBetweenAction;
    /***********************************
    CENTRER LA CAMERA SUR LA POSITION DU JOUEUR
    ***********************************/
    int x,y;
    SDL_GetWindowSize(window, &x, &y);

    leLayer->offsetX = x/2 - gameObject_getHitBox(&leLayer->player->gameObject)->x + gameObject_getHitBox(&leLayer->player->gameObject)->w/2;
    leLayer->offsetY = y/2 - gameObject_getHitBox(&leLayer->player->gameObject)->y + gameObject_getHitBox(&leLayer->player->gameObject)->h/2;

    //Trier par Y croissant
    int temp;
    int * nOrderPerso;
    nOrderPerso = (int*)malloc((leLayer->nbPerso+1)*sizeof(int));
    //int nOrderObject[objetMAX];
    for(int i = 0; i < leLayer->nbPerso; i++)
    {
        nOrderPerso[i] = i;
    }
    /*for(int i = 0; i <= leLayer->nbObject; i++)
    {
        nOrderObject[i] = i;
    }*/

    for(int i=0; i<leLayer->nbPerso; i++)
    {
        /*
         * Place currently selected element array[i]
         * to its correct place.
         */
        for(int j=i+1; j<leLayer->nbPerso; j++)
        {
            /*
             * Swap if currently selected array element
             * is not at its correct position.
             */
            if(leLayer->persos[nOrderPerso[i]].gameObject.boxCollider.y > leLayer->persos[nOrderPerso[j]].gameObject.boxCollider.y)
            {
                temp = nOrderPerso[i];
                nOrderPerso[i] = nOrderPerso[j];
                nOrderPerso[j] = temp;
            }
        }
    }



    for( int i = 0; i < leLayer->nbObject; i++)
    {
        object_DRAW(&leLayer->objects[i], renderer, leLayer->offsetX, leLayer->offsetY);
    }
    //Afficher les entités
    for( int i = 0; i < leLayer->nbPerso; i++)
    {
        perso_DRAW(&leLayer->persos[nOrderPerso[i]], renderer, leLayer->offsetX, leLayer->offsetY);
    }

    free(nOrderPerso);

    /**************
    Collisions de combat
    **************/
    for( int i = 0; i < leLayer->nbPerso; i++)
    {
        //Check collisions
        if(leLayer->persos[i].attacking == 1 && sprite_getFrame(perso_getAttackSprite(&leLayer->persos[i])) == leLayer->persos[i].attackFrame )
        {
            for(int j = 0; j < leLayer->nbPerso; j++)
            {
                if(i == j)
                {
                    continue;
                }

                if(SDL_HasIntersection(perso_getAttackBox(&leLayer->persos[i]), gameObject_getHitBox(&leLayer->persos[j].gameObject)))
                {
                    if(j!=0)
                        perso_setHitted(&leLayer->persos[j], &leLayer->persos[i], leLayer->persos[i].damage);
                    else
                        perso_setHitted(&leLayer->persos[j], NULL, leLayer->persos[i].damage);

                }
            }
        }

        //Check aggro target
        for(int j = 0; j < leLayer->nbPerso; j++)
        {
            if(i == j)
            {
                continue;
            }
            if(getDistanceBetween(&leLayer->persos[i], &leLayer->persos[j].gameObject) < getDistanceBetween(&leLayer->persos[i], leLayer->persos[i].pTarget)
                && gameObject_getLife(&leLayer->persos[j].gameObject) > 0
                && perso_getRaceAttitude(&leLayer->persos[i], leLayer->persos[j].race) < 0
                && getDistanceBetween(&leLayer->persos[i], &leLayer->persos[j].gameObject) < getDetecRange(leLayer->persos[i]))
            {
                perso_setTarget(&leLayer->persos[i], &leLayer->persos[j].gameObject);
            }
            if(leLayer->persos[i].pTarget != NULL && getDistanceBetween(&leLayer->persos[i], leLayer->persos[i].pTarget) > getDistanceBetween(&leLayer->persos[i], NULL))
            {
                perso_setTarget(&leLayer->persos[i], NULL);
            }
        }

        perso_Simulate(&leLayer->persos[i], ticks);
    }

    /***************************************
    VERIFIE LES COLLISIONS ENTRE LES PERSONNAGES
    ***************************************/
    for (int i = 0; i < leLayer->nbPerso; i++)
    {
        for(int j = 0; j < leLayer->nbPerso ; j++)
        {
            while(SDL_HasIntersection(gameObject_getBoxCollider(&leLayer->persos[i].gameObject), gameObject_getBoxCollider(&leLayer->persos[j].gameObject)) && leLayer->persos[i].attacking != 4 && leLayer->persos[j].attacking != 4)
            {
                if(i == j)
                {
                    j++;
                    continue;
                }
                // printf("Oui %i %i X1 : %f Y1 : %f, X2 : %f Y2 : %f / %f\n", leLayer->persos[i].gameObject.id, leLayer->persos[j].gameObject.id, leLayer->persos[i].gameObject.X, leLayer->persos[i].gameObject.Y, leLayer->persos[j].gameObject.X, leLayer->persos[j].gameObject.Y , ((leLayer->persos[i].gameObject.Y + leLayer->persos[i].gameObject.boxCollider.h/2) - (leLayer->persos[j].gameObject.Y + leLayer->persos[j].gameObject.boxCollider.h/2)));
                if(((leLayer->persos[i].gameObject.Y + leLayer->persos[i].gameObject.boxCollider.h/2) - (leLayer->persos[j].gameObject.Y + leLayer->persos[j].gameObject.boxCollider.h/2)) != 0.0)
                {

                    float direc1 = leLayer->persos[i].gameObject.boxCollider.w;
                    float direc2 = leLayer->persos[j].gameObject.boxCollider.w;
                    float direc3 = leLayer->persos[i].gameObject.boxCollider.h;
                    float direc4 = leLayer->persos[j].gameObject.boxCollider.h;
                    float direc = (fabs(((leLayer->persos[i].gameObject.X + direc1 / 2.0)
                                      - (leLayer->persos[j].gameObject.X + direc2 / 2.0))
									  / ((leLayer->persos[i].gameObject.Y + direc3 / 2.0)
									  - (leLayer->persos[j].gameObject.Y + direc4 / 2.0))*2/PI));

                    float rat1 = leLayer->persos[i].gameObject.boxCollider.h/2.0;
                    float rat2 = leLayer->persos[i].gameObject.boxCollider.w/2.0;
                    float rat = atan( fabs(rat1 / rat2) )*2.0/PI;

                    if(direc >= rat)
                    {
                        if(((leLayer->persos[i].gameObject.X + leLayer->persos[i].gameObject.boxCollider.w/2) - (leLayer->persos[j].gameObject.X + leLayer->persos[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X + 1, leLayer->persos[i].gameObject.Y);
                            perso_MOVETO(&leLayer->persos[j], leLayer->persos[j].gameObject.X - 1, leLayer->persos[j].gameObject.Y);
                        }
                        else
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X - 1, leLayer->persos[i].gameObject.Y);
                            perso_MOVETO(&leLayer->persos[j], leLayer->persos[j].gameObject.X + 1, leLayer->persos[j].gameObject.Y);
                        }

                    }
                    else if(direc < rat)
                    {
                        if(((leLayer->persos[i].gameObject.Y + leLayer->persos[i].gameObject.boxCollider.h/2) - (leLayer->persos[j].gameObject.Y + leLayer->persos[j].gameObject.boxCollider.h/2)) > 0)
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X, leLayer->persos[i].gameObject.Y + 1);
                            perso_MOVETO(&leLayer->persos[j], leLayer->persos[j].gameObject.X, leLayer->persos[j].gameObject.Y - 1);
                        }
                        else
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X, leLayer->persos[i].gameObject.Y - 1);
                            perso_MOVETO(&leLayer->persos[j], leLayer->persos[j].gameObject.X, leLayer->persos[j].gameObject.Y + 1);
                        }
                    }
                }
                else
                {
                        if(((leLayer->persos[i].gameObject.X + leLayer->persos[i].gameObject.boxCollider.w/2) - (leLayer->persos[j].gameObject.X + leLayer->persos[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X + 1, leLayer->persos[i].gameObject.Y);
                            perso_MOVETO(&leLayer->persos[j], leLayer->persos[j].gameObject.X - 1, leLayer->persos[j].gameObject.Y);
                        }
                        else
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X - 1, leLayer->persos[i].gameObject.Y);
                            perso_MOVETO(&leLayer->persos[j], leLayer->persos[j].gameObject.X + 1, leLayer->persos[j].gameObject.Y);
                        }

                }

            }
        }
    }

    /***********************************************************
    VERIFIE LES COLLISIONS ENTRE LES PERSONNAGES ET LES OBJECTS
    ***********************************************************/
    for (int i = 0; i < leLayer->nbPerso; i++)
    {
        for(int j = 0; j < leLayer->nbObject ; j++)
        {
            while(SDL_HasIntersection(gameObject_getBoxCollider(&leLayer->persos[i].gameObject), gameObject_getBoxCollider(&leLayer->objects[j].gameObject)) )
            {
                // printf("Oui %i  Object : %i X1 : %f Y1 : %f, X2 : %f Y2 : %f / %f\n", leLayer->persos[i].gameObject.id, leLayer->objects[j].gameObject.id, leLayer->persos[i].gameObject.X, leLayer->persos[i].gameObject.Y, leLayer->objects[j].gameObject.X, leLayer->objects[j].gameObject.Y , ((leLayer->persos[i].gameObject.Y + leLayer->persos[i].gameObject.boxCollider.h/2) - (leLayer->objects[j].gameObject.Y + leLayer->objects[j].gameObject.boxCollider.h/2)));
                if(((leLayer->persos[i].gameObject.Y + leLayer->persos[i].gameObject.boxCollider.h/2) - (leLayer->objects[j].gameObject.Y + leLayer->objects[j].gameObject.boxCollider.h/2)) != 0.0)
                {
                    float direc1 =  leLayer->persos[i].gameObject.boxCollider.w/2.0;
                    float direc2 = leLayer->objects[j].gameObject.boxCollider.w/2.0;
                    float direc3 =  leLayer->persos[i].gameObject.boxCollider.h/2.0;
                    float direc4 = leLayer->objects[j].gameObject.boxCollider.h/2.0;
                    float direc = atan(fabs(((leLayer->persos[i].gameObject.boxCollider.x + direc1)
                                          - (leLayer->objects[j].gameObject.boxCollider.x + direc2))
									      / ((leLayer->persos[i].gameObject.boxCollider.y + direc3)
									      - (leLayer->objects[j].gameObject.boxCollider.y + direc4)))) *2/PI;

                    float rat1 = leLayer->persos[i].gameObject.boxCollider.h/2.0;
                    float rat2 = leLayer->persos[i].gameObject.boxCollider.w/2.0;
                    float rat = atan( fabs( rat1 / rat2) )/90*180.0/PI;
                    if(direc >= rat)
                    {
                        if(((leLayer->persos[i].gameObject.boxCollider.x + leLayer->persos[i].gameObject.boxCollider.w/2) - (leLayer->objects[j].gameObject.boxCollider.x + leLayer->objects[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X + 1, leLayer->persos[i].gameObject.Y);
                        }
                        else
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X - 1, leLayer->persos[i].gameObject.Y);
                        }

                    }
                    else if(direc < rat)
                    {
                        if(((leLayer->persos[i].gameObject.boxCollider.y + leLayer->persos[i].gameObject.boxCollider.h/2) - (leLayer->objects[j].gameObject.boxCollider.y + leLayer->objects[j].gameObject.boxCollider.h/2)) > 0)
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X, leLayer->persos[i].gameObject.Y + 1);
                        }
                        else
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X, leLayer->persos[i].gameObject.Y - 1);
                        }
                    }

                }
                else
                {
                        if(((leLayer->persos[i].gameObject.X + leLayer->persos[i].gameObject.boxCollider.w/2) - (leLayer->objects[j].gameObject.X + leLayer->objects[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X + 1, leLayer->persos[i].gameObject.Y);
                        }
                        else
                        {
                            perso_MOVETO(&leLayer->persos[i], leLayer->persos[i].gameObject.X - 1, leLayer->persos[i].gameObject.Y);
                        }
                }

            }
        }
    }

}

void layer_Destroy(layer * leLayer)
{
    for(int i = 0; i < leLayer->nbPerso; i++)
    {
        SDL_DestroyTexture(leLayer->persos[i].sprAttack.texture);
        SDL_DestroyTexture(leLayer->persos[i].sprWalk.texture);
        SDL_DestroyTexture(leLayer->persos[i].sprIdle.texture);
        SDL_DestroyTexture(leLayer->persos[i].sprDead.texture);
        SDL_DestroyTexture(leLayer->persos[i].sprHit.texture);
    }
    for(int i = 0; i < leLayer->nbObject; i++)
    {
        SDL_DestroyTexture(leLayer->objects[i].sprIdle.texture);
    }
    free(leLayer->persos);
    free(leLayer->objects);
}

#endif // LAYER_H_INCLUDED
