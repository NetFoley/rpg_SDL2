#ifndef scene_H_INCLUDED
#define scene_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include "perso.h"
#include "interface.h"
#include "object.h"
#include "IA.h"
#include "weapon.h"
#include "caracterMatching.h"

#define PI 3.14159265

typedef struct scene{
    int offsetX;
    int offsetY;
	perso * persos;
	perso * modelPersos;
	object * objects;
	weapon * weapons;
	perso * player;
    Uint32 timeSinceLastAction;
    int timeBetweenAction;
	int nbPerso;
	int nbModelPerso;
	int nbObject;
	int nbWeapon;
	int idPlayer;
	interface interface;

	caracterMatching carDef;
	char imagePath[255];
    char sMap[255];
	SDL_bool debugMode;
	SDL_Renderer * renderer;
}scene;


void scene_INI(scene * lescene, SDL_Renderer * renderer);
void scene_DRAW(scene * lescene, SDL_Renderer * renderer, SDL_Window * window);
void scene_AddSoldier(scene * lescene, int x, int y);
void scene_AddSquelette(scene * lescene, int x, int y);
void scene_AddCoffre(scene * lescene, int x, int y);
void scene_AddGrosCoffre(scene * lescene);
void scene_getInput(scene * l, SDL_Event event);
void scene_Destroy();
void scene_readMap(scene * l, char sMap[255]);
void scene_readWeapons(scene * l);
void scene_readCharacters(scene * s);
void scene_setPlayer(scene * lay, perso * p);
perso * scene_getPersoById(scene * lay, int id);
char * scene_GetImgPath(scene * scene);
perso * scene_AddModelChar(scene * s);
weapon * scene_findWeapon(scene * s, char * weaponName);
perso * scene_findModelPerso(scene * s, char * modelName);

void scene_INI(scene * lescene, SDL_Renderer * renderer)
{
    lescene->offsetX = 0;
    lescene->offsetY = 0;

    lescene->persos =(perso*)malloc(sizeof(perso*));
    lescene->modelPersos =(perso*)malloc(sizeof(perso*));
    lescene->objects =(object*)malloc(sizeof(object*));
    lescene->weapons =(weapon*)malloc(sizeof(weapon*));

	lescene->nbPerso = 0;
	lescene->nbModelPerso = 0;
	lescene->nbObject = 0;
	lescene->nbWeapon = 0;

	lescene->timeSinceLastAction = SDL_GetTicks();
	lescene->timeBetweenAction = 30;
	lescene->idPlayer = 0;
	lescene->player = (perso*)malloc(sizeof(perso*));
	lescene->player = NULL;
	lescene->debugMode = SDL_FALSE;
	lescene->renderer = renderer;
	match_ini(&lescene->carDef);
    scene_readWeapons(lescene);
	scene_readCharacters(lescene);

	interface_INI(&lescene->interface, renderer);

}


void scene_DefinePath(scene * scene, char path[255])
{
    strcpy(scene->imagePath, path);
}

char * scene_GetImgPath(scene * scene)
{
    return scene->imagePath;
}

void scene_AddCharacter(scene * s, char name[255], int x, int y)
{
    s->persos =(perso*)realloc(s->persos, (s->nbPerso+1)*(sizeof(perso)));
    perso_INI(&s->persos[s->nbPerso], s->renderer, scene_findModelPerso(s, name), x, y);
    s->nbPerso++;
    //printf("%i loaded perso\n\n", lescene->nbPerso);
}

perso * scene_AddModelChar(scene * s)
{
    s->modelPersos =(perso*)realloc(s->modelPersos, (s->nbModelPerso+1)*(sizeof(perso)));
    s->nbModelPerso++;
    return &s->modelPersos[s->nbModelPerso - 1];
}

void scene_AddWeapon(scene * l, char name[255], weaponType weaponType, int damage, int h, int w)
{
    l->weapons =(weapon*)realloc(l->weapons, (l->nbWeapon+1)*(sizeof(weapon)));
    printf("Adding weapon '%s' with weaponType(%i) damage(%i)\n", name, weaponType, damage);
    INIT_weapon(&l->weapons[l->nbWeapon], name, weaponType, damage, h, w);
    l->nbWeapon++;
}

void scene_AddCoffre(scene * lescene, int x, int y)
{
    //CA VA REBUGGER CORRIGER LA MEMOIRE AJOUTE AVEC LE BLOCK DE PATHFINDING
    lescene->objects =(object*)realloc(lescene->objects, (lescene->nbObject+1)*(sizeof(object)));
    object_INI(&lescene->objects[lescene->nbObject], lescene, lescene->renderer, 0, x ,y);
    lescene->nbObject++;
    //printf("%i loaded objects\n\n", lescene->nbObject);
}

/*void scene_AddGrosCoffre(scene * lescene, SDL_Renderer * renderer)
{
    if(lescene->nbPerso < objetMAX)
    {
        object_INI(&lescene->objects[lescene->nbObject], renderer, 1);
        lescene->nbObject++;
    }
}*/

void scene_setImagePath(scene * s, char path[255])
{
    strcpy(s->imagePath, path);
}

void scene_readCharacters(scene * s)
{
    FILE * pFile;
    char text[255];
    strcpy(text, CHARACPATH);
    strcat(text, "characters.txt");
    pFile = fopen(text, "r");
    if (pFile != NULL)
    {
        char buff[255];
        while(fscanf(pFile, "%s", buff) == 1)
        {

            FILE * pfCharacters;
            strcpy(text, CHARACPATH);
            strcat(text, buff);
            strcat(text, ".txt");
            pfCharacters = fopen(text, "r");

            char charName[255];
            strcpy(charName, buff);
            char car = 'a';
            if(fscanf(pFile, "%s", buff) == 1)
            {
                car = buff[0];
            }
            else{printf("FUCK CAR\n");}
            match_file(&s->carDef, charName, car);

            if(pfCharacters != NULL)
            {
                //STATS
                char name[255];
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    strcpy(name, buff);
                }
                        else{printf("FUCK\n");}


                char weapon[255];
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    strcpy(weapon, buff);
                }
                        else{printf("FUCK\n");}

                int maxLife;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &maxLife);
                }
                        else{printf("FUCK\n");}
                float maxV ;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%f", &maxV );
                }
                        else{printf("FUCK\n");}
                float acc;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%f", &acc);
                }
                        else{printf("FUCK\n");}
                int movable;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &movable);
                }
                int passable;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &passable);
                }
                        else{printf("FUCK\n");}
                int race;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &race);
                }
                        else{printf("FUCK\n");}
                int attackFrame;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &attackFrame );
                }
                        else{printf("FUCK\n");}
                int range;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &range  );
                }
                        else{printf("FUCK\n");}
                int hitRange;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &hitRange );
                }
                        else{printf("FUCK\n");}

                //SPRITES
                char tPerso[dead + 1][255];
                for(int i = 0; i < dead +1 ; i++)
                    strcpy(tPerso[i], name);
                strcat(tPerso[idle], "_Idle.png");
                strcat(tPerso[walk], "_Walk.png");
                strcat(tPerso[attack], "_Attack.png");
                strcat(tPerso[block], "_Block.png");
                strcat(tPerso[hit], "_Hit.png");
                strcat(tPerso[dead], "_Dead.png");

                int nSprite[4];
                sprite sprites[dead+1];
                for(int j = 0; j < dead + 1; j++)
                {
                    for(int i = 0; i < 4; i++)
                    {
                        if(fscanf(pfCharacters, "%s", buff) == 1)
                        {
                            sscanf(buff, "%d", &nSprite[i] );
                        }
                        else{printf("FUCK\n");}
                    }
                    sprite_INI(&sprites[j], tPerso[j], nSprite[0], nSprite[1], nSprite[2], s->renderer);
                    sprite_setTickInBetween(&sprites[j], nSprite[3]);
                }

            //INITIALISATION
			perso * newPerso = scene_AddModelChar(s);
			printf("New character with name (%s) maxLife(%i) weapon (%s)\n", name, maxLife, weapon);
			newModel_INI(newPerso, name, maxLife, maxV, acc, movable, passable, race, attackFrame, range, hitRange, sprites, scene_findWeapon(s, weapon));
            fclose(pfCharacters);
            }
        }

        fclose(pFile);
        printf("\n");
    }
    else
    {
        printf("INI: Aucun character trouve\n");
    }

}

weapon * scene_findWeapon(scene * s, char * weaponName)
{
    weapon * w = NULL;
	for(int i = 0; i < s->nbWeapon; i++)
    {
        if(strcmp(s->weapons[i].name, weaponName) == 0)
        {
            w = &s->weapons[i];
        }
    }
    return w;
}

perso * scene_findModelPerso(scene * s, char modelName[255])
{
	for(int i = 0; i < s->nbModelPerso; i++)
    {
        if(strcmp(s->modelPersos[i].name, modelName) == 0)
        {
            return &s->modelPersos[i];
        }
    }
    return NULL;
}

void scene_readWeapons(scene * s)
{
    FILE * pFile;
    char text[50];
    strcpy(text, WEAPONPATH);
    strcat(text, "weapons.txt");
    pFile = fopen(text, "r");
    if (pFile != NULL)
    {
        char buff[255];
        while(fscanf(pFile, "%s", buff) == 1)
        {
            FILE * pfWeapon;
            strcpy(text, WEAPONPATH);
            strcat(text, buff);
            strcat(text, ".txt");
            pfWeapon = fopen(text, "r");

            char a[255];
            char name[255];
            if(fscanf(pfWeapon, "%s", a) == 1)
            {
                strcpy(name, a);
            }
            weaponType weaponType;
            int type;
            if(fscanf(pfWeapon, "%s", a) == 1)
            {
                sscanf(a, "%d", &type);
                weaponType = type;
            }
            int damage;
            if(fscanf(pfWeapon, "%s", a) == 1)
            {
                sscanf(a, "%d", &damage);
            }
            int h;
            if(fscanf(pfWeapon, "%s", a) == 1)
            {
                sscanf(a, "%d", &h);
            }
            int w;
            if(fscanf(pfWeapon, "%s", a) == 1)
            {
                sscanf(a, "%d", &w);
            }

            scene_AddWeapon(s, name, weaponType, damage, h, w);
            fclose(pfWeapon);
        }
        fclose(pFile);
        printf("\n");
    }
    else
    {
        printf("INI: Aucune arme trouve\n");
    }

}

void scene_readMap(scene * s, char sMap[255])
{
    FILE * pFile;
    strcpy(s->sMap, sMap);
    pFile = fopen(sMap, "r");
    if (pFile != NULL)
    {
        char a = ' ';
        int i = 0, j = 0;
        while(a != EOF)
        {
            a = fgetc(pFile);
            switch (a)
            {
            case 'c' :
                scene_AddCoffre(s, i*TILESIZE, j*TILESIZE);
                break;
            case 'P' :
                scene_AddCharacter(s, s->carDef.names[0], i*TILESIZE, j*TILESIZE);
                scene_setPlayer(s, &s->persos[s->nbPerso-1]);
                printf("Player id : %i\n", s->persos[s->nbPerso-1].gameObject.id);
                break;
            case '\n' :
                j++;
                i = -1;
                break;
            default:
                for(int k = 0; k < s->carDef.nbr; k++)
                {
                    if(s->carDef.caracters[k] == a)
                    {
                        scene_AddCharacter(s, s->carDef.names[k], i*TILESIZE, j*TILESIZE);
                    }
                }
            }
            i++;
        }
        fclose(pFile);
    }

}

void scene_setPlayer(scene * lay, perso * p)
{
    lay->idPlayer = p->gameObject.id;
    p->playerControled = SDL_TRUE;
}

perso * scene_getPersoById(scene * lay, int id)
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

void scene_getInput(scene * l, SDL_Event event)
/**************************************
GESTION DES ENTREES CLAVIER
***************************************/
{
    if(l->player == NULL)
    {
        l->player = scene_getPersoById(l, l->idPlayer);
    }
    if(l->player->gameObject.id != l->idPlayer)
    {
        l->player = scene_getPersoById(l, l->idPlayer);
    }


    if(l->player->gameObject.life > 0 )
    {
        /*
        if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED) {
            moveToMouse(&l->persos[0].gameObject);
            gameObject_addLife(&l->persos[0].gameObject,10);
        }*/
        static SDL_bool leftPressed = SDL_FALSE;
        static SDL_bool rightPressed = SDL_FALSE;
        static SDL_bool middlePressed = SDL_FALSE;
        SDL_Point mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        mousePos.x -= l->offsetX;
        mousePos.y -= l->offsetY;
        SDL_Point pureMousePos;
        SDL_GetMouseState(&pureMousePos.x, &pureMousePos.y);

        if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED && leftPressed == SDL_FALSE)
        {
            leftPressed = SDL_TRUE;

            for(int i = 0; i < l->nbPerso; i++)
            {
                if(SDL_PointInRect(&mousePos, gameObject_getBoxCollider(&l->persos[i].gameObject)))
                {
                    if(l->debugMode == SDL_TRUE)
                    {
                        printf("CLICK ON OBJECT id:%i \n", l->persos[i].gameObject.id);
                        printf("name: %s \n", l->persos[i].name);
                        switch(l->persos[i].gameObject.direction){
                        {case north: printf("direction: north \n");break;}
                        {case south: printf("direction: south \n");break;}
                        {case west: printf("direction: west \n");break;}
                        {case east: printf("direction: east \n");}break;}
                        if(l->persos[i].path != NULL)
                        {
                            printf("Going to ");
                            if(l->persos[i].path->premier != NULL)
                                printf("X%i Y%i.\n", l->persos[i].path->premier->nombre.x, l->persos[i].path->premier->nombre.y);
                            else
                                printf("nowhere.\n");
                        }
                        printf("Neighbours : %i \n", l->persos[i].nbNeighbours);
                        printf("goal vx %f vy %f \n", l->persos[i].gameObject.speed.x, l->persos[i].gameObject.coord.y);
                        printf("\n");
                    }
                }
            }
        }
        else if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED)
        {
            leftPressed = SDL_FALSE;
        }
        if(event.button.button == SDL_BUTTON_RIGHT && event.button.state == SDL_PRESSED && rightPressed == SDL_FALSE)
        {
            rightPressed = SDL_TRUE;
            int charFound = -1;
            for(int i = 0; i < l->nbPerso; i++)
            {
                if(SDL_PointInRect(&mousePos, gameObject_getBoxCollider(&l->persos[i].gameObject)))
                {
                    printf("RIGHT CLICK ON OBJECT id:%i \n", l->persos[i].gameObject.id);
                    charFound = i;
                }
            }
            if(charFound != -1)
            {
                interface_afficherOptions(&l->interface, l->persos[charFound], l->offsetX, l->offsetY);
            }
            else
            {
                interface_cacherOptions(&l->interface);
            }
        }
        else if(event.button.button == SDL_BUTTON_RIGHT && event.button.state == SDL_RELEASED)
        {
            rightPressed = SDL_FALSE;
        }
        for(int i = 0; i < l->interface.nbOptions; i++)
        {
            if(SDL_PointInRect(&pureMousePos, interface_getOptionBox(&l->interface, i)))
            {
                l->interface.selection = i;
            }
        }
        if(event.button.button == SDL_BUTTON_MIDDLE && event.button.state == SDL_PRESSED && middlePressed == SDL_FALSE)
        {
        }
        else if(event.button.button == SDL_BUTTON_RIGHT && event.button.state == SDL_RELEASED)
        {
            middlePressed = SDL_FALSE;
        }

            static SDL_bool TKEYPRESSED[SDL_NUM_SCANCODES] = {0};
            switch(event.type){
                case SDL_KEYDOWN:
                    //Entrées au clavier
                    if(TKEYPRESSED[SDL_SCANCODE_F3] == SDL_FALSE && event.key.keysym.scancode == SDL_SCANCODE_F3)
                    {
                        if(l->debugMode == SDL_TRUE)
                        {
                            l->debugMode = SDL_FALSE;
                            printf("Debug mod OFF\n");
                            printf("\n");
                        }
                        else if(l->debugMode == SDL_FALSE)
                        {
                            l->debugMode = SDL_TRUE;
                            printf("Debug mod ON\n");
                            printf("\n");
                        }
                    }
                    TKEYPRESSED[event.key.keysym.scancode] = SDL_TRUE;
                break;

                case SDL_KEYUP:
                    TKEYPRESSED[event.key.keysym.scancode] = SDL_FALSE;
                break;

            }

            if(TKEYPRESSED[SDL_SCANCODE_W] && l->player->statue != attack)
            {
                perso_moveUp(l->player);
            }
            else if(TKEYPRESSED[SDL_SCANCODE_S] && l->player->statue != attack)
            {
                perso_moveDown(l->player);
            }
            else
            {
                perso_slowY(l->player);
            }
            if(TKEYPRESSED[SDL_SCANCODE_A] && l->player->statue != attack)
            {
                perso_moveLeft(l->player);
            }
            else if(TKEYPRESSED[SDL_SCANCODE_D] && l->player->statue != attack)
            {
                perso_moveRight(l->player);
            }
            else
            {
                perso_slowX(l->player);
            }
            if (TKEYPRESSED[SDL_SCANCODE_F]) {
                perso_setStatue(l->player, attack, NULL);
            }
            if (TKEYPRESSED[SDL_SCANCODE_R]) {
                perso_setStatue(l->player, block, NULL);
            }


    }
}

void scene_DRAW(scene * lescene, SDL_Renderer * renderer, SDL_Window * window)
{
    Uint32 ticks = SDL_GetTicks();
    while(ticks - lescene->timeSinceLastAction < lescene->timeBetweenAction)
    {
        ticks = SDL_GetTicks();
    }
    lescene->timeSinceLastAction += lescene->timeBetweenAction;
    /***********************************
    CENTRER LA CAMERA SUR LA POSITION DU JOUEUR
    ***********************************/
    int x,y;
    SDL_GetWindowSize(window, &x, &y);

    lescene->offsetX = x/2 - lescene->player->gameObject.coord.x - TILESIZE/2;
    lescene->offsetY = y/2 - lescene->player->gameObject.coord.y - TILESIZE/2;

    //Trier par Y croissant
    int temp;
    int * nOrderPerso;
    nOrderPerso = (int*)malloc((lescene->nbPerso+1)*sizeof(int));
    //int nOrderObject[objetMAX];
    for(int i = 0; i < lescene->nbPerso; i++)
    {
        nOrderPerso[i] = i;
    }
    /*for(int i = 0; i <= lescene->nbObject; i++)
    {
        nOrderObject[i] = i;
    }*/

    for(int i=0; i<lescene->nbPerso; i++)
    {
        /*
         * Place currently selected element array[i]
         * to its correct place.
         */
        for(int j=i+1; j<lescene->nbPerso; j++)
        {
            /*
             * Swap if currently selected array element
             * is not at its correct position.
             */
            if(lescene->persos[nOrderPerso[i]].gameObject.boxCollider.y > lescene->persos[nOrderPerso[j]].gameObject.boxCollider.y)
            {
                temp = nOrderPerso[i];
                nOrderPerso[i] = nOrderPerso[j];
                nOrderPerso[j] = temp;
            }
        }
    }



    for( int i = 0; i < lescene->nbObject; i++)
    {
        object_DRAW(&lescene->objects[i], renderer, lescene->offsetX, lescene->offsetY);
    }
    //Afficher les entités
    for( int i = 0; i < lescene->nbPerso; i++)
    {
        perso_DRAW(&lescene->persos[nOrderPerso[i]], renderer, lescene->offsetX, lescene->offsetY, lescene->debugMode);
    }

    /**************
    Collisions de combat
    **************/
    for( int i = 0; i < lescene->nbPerso; i++)
    {
        //Check collisions
        if(lescene->persos[i].statue == attack && sprite_getFrame(perso_getSprite(&lescene->persos[i], attack)) == lescene->persos[i].attackFrame )
        {
            for(int j = 0; j < lescene->nbPerso; j++)
            {
                if(i == j)
                {
                    continue;
                }

                if(SDL_HasIntersection(perso_getAttackBox(&lescene->persos[i]), gameObject_getBoxCollider(&lescene->persos[j].gameObject)))
                {
                    perso_setStatue(&lescene->persos[j], hit, &lescene->persos[i]);
                }
            }
        }

        //Check aggro target
        for(int j = 0; j < lescene->nbPerso; j++)
        {
            if(i == j)
            {
                continue;
            }
            if(getDistanceBetween(&lescene->persos[i].gameObject, &lescene->persos[j].gameObject) < getDistanceBetween(&lescene->persos[i].gameObject, lescene->persos[i].pTarget)
                && gameObject_getLife(&lescene->persos[j].gameObject) > 0
                && perso_getRaceAttitude(&lescene->persos[i], lescene->persos[j].race) < 0
                && getDistanceBetween(&lescene->persos[i].gameObject, &lescene->persos[j].gameObject) < getDetecRange(lescene->persos[i]))
            {
                perso_setTarget(&lescene->persos[i], &lescene->persos[j].gameObject);
            }
            if(lescene->persos[i].pTarget != NULL && getDistanceBetween(&lescene->persos[i].gameObject, lescene->persos[i].pTarget) > getDistanceBetween(&lescene->persos[i].gameObject, NULL))
            {
                perso_setTarget(&lescene->persos[i], NULL);
            }
        }
        perso_getFriends(&lescene->persos[i], lescene->persos, lescene->nbPerso);
        perso_Simulate(&lescene->persos[i], lescene->sMap, ticks);
    }

    /***************************************
    VERIFIE LES COLLISIONS ENTRE LES PERSONNAGES
    ***************************************/
    for (int i = 0; i < lescene->nbPerso; i++)
    {
        for(int j = 0; j < lescene->nbPerso ; j++)
        {
            if(i == j || lescene->persos[i].gameObject.passable == 1 || lescene->persos[j].gameObject.passable == 1)
            {
                break;
            }
            while(SDL_HasIntersection(gameObject_getBoxCollider(&lescene->persos[i].gameObject), gameObject_getBoxCollider(&lescene->persos[j].gameObject)) && lescene->persos[i].statue != dead && lescene->persos[j].statue != dead)
            {
                // printf("Oui %i %i X1 : %f Y1 : %f, X2 : %f Y2 : %f / %f\n", lescene->persos[i].gameObject.id, lescene->persos[j].gameObject.id, lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y, lescene->persos[j].gameObject.coord.x, lescene->persos[j].gameObject.coord.y , ((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->persos[j].gameObject.coord.y + lescene->persos[j].gameObject.boxCollider.h/2)));
                if(((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->persos[j].gameObject.coord.y + lescene->persos[j].gameObject.boxCollider.h/2)) != 0.0)
                {

                    float direc1 = lescene->persos[i].gameObject.boxCollider.w;
                    float direc2 = lescene->persos[j].gameObject.boxCollider.w;
                    float direc3 = lescene->persos[i].gameObject.boxCollider.h;
                    float direc4 = lescene->persos[j].gameObject.boxCollider.h;
                    float direc = (fabs(((lescene->persos[i].gameObject.coord.x + direc1 / 2.0)
                                      - (lescene->persos[j].gameObject.coord.x + direc2 / 2.0))
									  / ((lescene->persos[i].gameObject.coord.y + direc3 / 2.0)
									  - (lescene->persos[j].gameObject.coord.y + direc4 / 2.0))*2/PI));

                    float rat1 = lescene->persos[i].gameObject.boxCollider.h/2.0;
                    float rat2 = lescene->persos[i].gameObject.boxCollider.w/2.0;
                    float rat = atan( fabs(rat1 / rat2) )*2.0/PI;

                    if(direc >= rat)
                    {
                        if(((lescene->persos[i].gameObject.coord.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->persos[j].gameObject.coord.x + lescene->persos[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x - 1, lescene->persos[j].gameObject.coord.y);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x + 1, lescene->persos[j].gameObject.coord.y);
                        }

                    }
                    else if(direc < rat)
                    {
                        if(((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->persos[j].gameObject.coord.y + lescene->persos[j].gameObject.boxCollider.h/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y + 1);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x, lescene->persos[j].gameObject.coord.y - 1);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y - 1);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x, lescene->persos[j].gameObject.coord.y + 1);
                        }
                    }
                }
                else
                {
                        if(((lescene->persos[i].gameObject.coord.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->persos[j].gameObject.coord.x + lescene->persos[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x - 1, lescene->persos[j].gameObject.coord.y);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x + 1, lescene->persos[j].gameObject.coord.y);
                        }

                }

            }
        }
    }

    /***********************************************************
    VERIFIE LES COLLISIONS ENTRE LES PERSONNAGES ET LES OBJECTS
    ***********************************************************/
    for (int i = 0; i < lescene->nbPerso; i++)
    {
        for(int j = 0; j < lescene->nbObject ; j++)
        {
            while(SDL_HasIntersection(gameObject_getBoxCollider(&lescene->persos[i].gameObject), gameObject_getBoxCollider(&lescene->objects[j].gameObject)) )
            {
                // printf("Oui %i  Object : %i X1 : %f Y1 : %f, X2 : %f Y2 : %f / %f\n", lescene->persos[i].gameObject.id, lescene->objects[j].gameObject.id, lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y, lescene->objects[j].gameObject.coord.x, lescene->objects[j].gameObject.coord.y , ((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->objects[j].gameObject.coord.y + lescene->objects[j].gameObject.boxCollider.h/2)));
                if(((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->objects[j].gameObject.coord.y + lescene->objects[j].gameObject.boxCollider.h/2)) != 0.0)
                {
                    float direc1 =  lescene->persos[i].gameObject.boxCollider.w/2.0;
                    float direc2 = lescene->objects[j].gameObject.boxCollider.w/2.0;
                    float direc3 =  lescene->persos[i].gameObject.boxCollider.h/2.0;
                    float direc4 = lescene->objects[j].gameObject.boxCollider.h/2.0;
                    float direc = atan(fabs(((lescene->persos[i].gameObject.boxCollider.x + direc1)
                                          - (lescene->objects[j].gameObject.boxCollider.x + direc2))
									      / ((lescene->persos[i].gameObject.boxCollider.y + direc3)
									      - (lescene->objects[j].gameObject.boxCollider.y + direc4)))) *2/PI;

                    float rat1 = lescene->persos[i].gameObject.boxCollider.h/2.0;
                    float rat2 = lescene->persos[i].gameObject.boxCollider.w/2.0;
                    float rat = atan( fabs( rat1 / rat2) )/90*180.0/PI;
                    if(direc >= rat)
                    {
                        if(((lescene->persos[i].gameObject.boxCollider.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->objects[j].gameObject.boxCollider.x + lescene->objects[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                        }

                    }
                    else if(direc < rat)
                    {
                        if(((lescene->persos[i].gameObject.boxCollider.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->objects[j].gameObject.boxCollider.y + lescene->objects[j].gameObject.boxCollider.h/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y + 1);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y - 1);
                        }
                    }

                }
                else
                {
                        if(((lescene->persos[i].gameObject.coord.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->objects[j].gameObject.coord.x + lescene->objects[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                        }
                }

            }
        }
    }
    interface_draw(&lescene->interface);

}


void scene_Destroy(scene * lescene)
{
    perso_destroy(&lescene->persos[0]);
    perso_destroy(&lescene->modelPersos[0]);
    for(int i = 0; i < lescene->nbObject; i++)
    {
        SDL_DestroyTexture(lescene->objects[i].sprIdle.texture);
    }
    carMatch_destroy(&lescene->carDef);
    free(lescene->modelPersos);
    free(lescene->weapons);
    free(lescene->persos);
    free(lescene->objects);
    free(lescene->player);
}

#endif // scene_H_INCLUDED
