#include "scene.h"

void scene_INI(scene * lescene, SDL_Renderer * renderer)
{
    lescene->mousePos.x = 0;
    lescene->mousePos.y = 0;
    lescene->offsetX = 0;
    lescene->offsetY = 0;

    lescene->persos =(perso*)malloc(sizeof(perso*));
    lescene->modelPersos =(perso*)malloc(sizeof(perso*));
    lescene->modelObjects =(object*)malloc(sizeof(object*));
    lescene->modelGroundTiles =(groundTile*)malloc(sizeof(groundTile*));
    lescene->objects =(object*)malloc(sizeof(object*));
    lescene->groundTiles =(groundTile*)malloc(sizeof(groundTile*));
    lescene->items =(item*)malloc(sizeof(item*));

	lescene->nbPerso = 0;
	lescene->nbModelPerso = 0;
	lescene->nbModelObject = 0;
	lescene->nbModelGroundTile = 0;
	lescene->nbGroundTile = 0;
	lescene->nbObject = 0;
	lescene->nbItem = 0;

	lescene->timeSinceLastAction = SDL_GetTicks();
	lescene->timeBetweenAction = 30;
	lescene->idPlayer = 0;
	lescene->player = (perso*)malloc(sizeof(perso*));
	lescene->player = NULL;
	lescene->debugMode = SDL_FALSE;
	lescene->renderer = renderer;
	match_ini(&lescene->carDef);
    scene_readItems(lescene);
	scene_readCharacters(lescene);
	scene_readObjects(lescene);
	scene_readGroundTiles(lescene);

	interface_INI(&lescene->interface, &lescene->offsetX, &lescene->offsetY, renderer);
    interface_addOption(&lescene->interface, "name", NULL);
    interface_addOption(&lescene->interface, "make vassal", perso_setLiege);
    interface_addOption(&lescene->interface, "order follow", perso_makeFollow);
    interface_addOption(&lescene->interface, "all follow", perso_makeFollowAll);
    interface_addOption(&lescene->interface, "order stop", perso_makeWander);
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
    s->persos =(perso*)realloc(s->persos, (s->nbPerso+1)*sizeof(perso));
    perso_INI(&s->persos[s->nbPerso], s, s->renderer, scene_findModelPerso(s, name), x, y);
    s->nbPerso++;
    //printf("%i loaded perso\n\n", lescene->nbPerso);
}

perso * scene_AddModelChar(scene * s)
{
    s->modelPersos =(perso*)realloc(s->modelPersos, (s->nbModelPerso+1)*sizeof(perso));
    s->nbModelPerso++;
    return &s->modelPersos[s->nbModelPerso - 1];
}

object * scene_AddModelObject(scene * s)
{
    s->modelObjects =(object*)realloc(s->modelObjects, (s->nbModelObject+1)*sizeof(object));
    s->nbModelObject++;
    return &s->modelObjects[s->nbModelObject - 1];
}

groundTile * scene_AddModelGroundTile(scene * s)
{
    s->modelGroundTiles =(groundTile*)realloc(s->modelGroundTiles, (s->nbModelGroundTile+1)*sizeof(groundTile));
    s->nbModelGroundTile++;
    return &s->modelGroundTiles[s->nbModelGroundTile - 1];
}

void scene_AddObject(scene * s, char name[255], int x, int y)
{
    s->objects =(object*)realloc(s->objects, (s->nbObject+1)*(sizeof(object)));
    object_INI(&s->objects[s->nbObject], scene_findModelObject(s, name), x ,y);
    s->nbObject++;
    refreshPaths(s->persos, s->nbPerso);
}

void scene_AddGroundTile(scene * s, char name[255], groundState state, float growth, int x, int y)
{
    s->groundTiles =(groundTile*)realloc(s->groundTiles, (s->nbGroundTile+1)*(sizeof(groundTile)));
    groundTile_INI(&s->groundTiles[s->nbGroundTile], scene_findModelGroundTile(s, name), state, growth, x ,y);
    s->nbGroundTile++;
}

void scene_AddWeapon(scene * l, char name[255], itemType itemType, weaponType weaponType, int damage, int h, int w)
{
    l->items =(item*)realloc(l->items, (l->nbItem+1)*(sizeof(item)));
    INIT_item(&l->items[l->nbItem], name, itemType, weaponType, damage, h, w);
    l->nbItem++;
}

void refreshPaths(perso * persos, int nbPerso)
{
    for(int i = 0; i < nbPerso; i++)
        viderFile(persos[i].path);
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

                float mass;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%f", &mass);
                }
                        else{printf("FUCK\n");}
                float str ;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%f", &str );
                }
                        else{printf("FUCK\n");}
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
                int intelligence;
                if(fscanf(pfCharacters, "%s", buff) == 1)
                {
                    sscanf(buff, "%d", &intelligence );
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
                strcat(tPerso[farm], "_Farm.png");
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
                    sprite_setTotalTime(&sprites[j], nSprite[3]);
                }

            //INITIALISATION
			perso * newPerso = scene_AddModelChar(s);
			newModel_INI(newPerso, name, mass, str, passable, race, attackFrame, range, hitRange, sprites, scene_findItem(s, weapon), intelligence);
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

void scene_readObjects(scene * s)
{
    FILE * pFile;
    char text[255];
    strcpy(text, OBJECTPATH);
    strcat(text, "objects.txt");
    pFile = fopen(text, "r");
    if (pFile != NULL)
    {
        char buff[255];
        while(fscanf(pFile, "%s", buff) == 1)
        {

            FILE * pfObjects;
            strcpy(text, OBJECTPATH);
            strcat(text, buff);
            strcat(text, ".txt");
            pfObjects = fopen(text, "r");

            char charName[255];
            strcpy(charName, buff);
            char car = 'a';
            if(fscanf(pFile, "%s", buff) == 1)
            {
                car = buff[0];
            }
            else{printf("FUCK OBJ\n");}
            match_file(&s->carDef, charName, car);

            if(pfObjects != NULL)
            {
                //STATS
                char name[255];
                if(fscanf(pfObjects, "%s", buff) == 1)
                {
                    strcpy(name, buff);
                }
                        else{printf("FUCK OBJ\n");}

                //SPRITES
                char tObject[1][255];
                for(int i = 0; i < 1 ; i++)
                    strcpy(tObject[i], name);
                strcat(tObject[idle], ".png");

                int nSprite[4];
                sprite sprites[1];
                for(int j = 0; j < 1; j++)
                {
                    for(int i = 0; i < 4; i++)
                    {
                        if(fscanf(pfObjects, "%s", buff) == 1)
                        {
                            sscanf(buff, "%d", &nSprite[i] );
                        }
                        else{printf("FUCK OBJ\n");}
                    }
                    sprite_INI(&sprites[j], tObject[j], nSprite[0], nSprite[1], nSprite[2], s->renderer);
                    sprite_setTotalTime(&sprites[j], nSprite[3]);
                }

            //INITIALISATION
			object * newObject = scene_AddModelObject(s);
			printf("New object named %s\n", name);
			modelObject_INI(newObject, name, sprites);
            fclose(pfObjects);
            }
        }

        fclose(pFile);
        printf("\n");
    }
    else
    {
        printf("INI: Aucun object trouve\n");
    }

}


void scene_readGroundTiles(scene * s)
{
    FILE * pFile;
    char text[255];
    strcpy(text, GROUNDPATH);
    strcat(text, "groundTiles.txt");
    pFile = fopen(text, "r");
    if (pFile != NULL)
    {
        char buff[255];
        while(fscanf(pFile, "%s", buff) == 1)
        {
            FILE * pfGroundTiles;
            strcpy(text, GROUNDPATH);
            strcat(text, buff);
            strcat(text, ".txt");
            pfGroundTiles = fopen(text, "r");

            if(pfGroundTiles != NULL)
            {
                //STATS
                char name[255];
                if(fscanf(pfGroundTiles, "%s", buff) == 1)
                {
                    strcpy(name, buff);
                }
                        else{printf("FUCK GROUDNTILE\n");}

                //SPRITES
                char tGroundTile[field+1][255];

                strcpy(tGroundTile[herb], "herb");
                strcat(tGroundTile[herb], ".png");
                strcpy(tGroundTile[flower], "flower");
                strcat(tGroundTile[flower], ".png");
                strcpy(tGroundTile[dirt], "dirt");
                strcat(tGroundTile[dirt], ".png");
                strcpy(tGroundTile[seed], "field");
                strcat(tGroundTile[seed], ".png");
                strcpy(tGroundTile[field], "grownField");
                strcat(tGroundTile[field], ".png");

                int nSprite;
                sprite sprites[field+1];
                for(int j = 0; j < field+1; j++)
                {
                    if(fscanf(pfGroundTiles, "%s", buff) == 1)
                    {
                        sscanf(buff, "%d", &nSprite );
                    }
                    else{printf("FUCK GROUDNTILE\n");}
                    sprite_INI(&sprites[j], tGroundTile[j], 0, 0, 16, s->renderer);
                    sprite_setTotalTime(&sprites[j], nSprite);
                }

            //INITIALISATION
			groundTile * newGroundTile = scene_AddModelGroundTile(s);
			printf("New groundTile named %s\n", name);
			modelGroundTile_INI(newGroundTile, name, sprites);
            fclose(pfGroundTiles);
            }
        }

        fclose(pFile);
        printf("\n");
    }
    else
    {
        printf("INI: Aucun groundTile trouve\n");
    }

}


item * scene_findItem(scene * s, char * itemName)
{
    item * w = NULL;
	for(int i = 0; i < s->nbItem; i++)
    {
        if(strcmp(s->items[i].name, itemName) == 0)
        {
            w = &s->items[i];
        }
    }
    if(w == NULL)
    {
        printf("No %s found ! \n", itemName);
    }
    return w;
}

perso * scene_findModelPerso(scene * s, char modelName[255])
{
	for(int i = 0; i < s->nbModelPerso; i++)
    {
        if(strcmp(s->modelPersos[i].gameObject.name, modelName) == 0)
        {
            return &s->modelPersos[i];
        }
    }
    return NULL;
}

object * scene_findModelObject(scene * s, char modelName[255])
{
	for(int i = 0; i < s->nbModelObject; i++)
    {
        if(strcmp(s->modelObjects[i].gameObject.name, modelName) == 0)
        {
            return &s->modelObjects[i];
        }
    }
    return NULL;
}

groundTile * scene_findModelGroundTile(scene * s, char modelName[255])
{
	for(int i = 0; i < s->nbModelGroundTile; i++)
    {
        if(strcmp(s->modelGroundTiles[i].name, modelName) == 0)
        {
            return &s->modelGroundTiles[i];
        }
    }
    return NULL;
}


void scene_readItems(scene * s)
{
    FILE * pFile;
    char text[50];
    strcpy(text, WEAPONPATH);
    strcat(text, "items.txt");
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
            itemType itemType;
            int iType;
            if(fscanf(pfWeapon, "%s", a) == 1)
            {
                sscanf(a, "%d", &iType);
                itemType = iType;
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

            scene_AddWeapon(s, name, itemType, weaponType, damage, h, w);
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
                scene_AddObject(s, "wall", i*TILESIZE, j*TILESIZE);
                scene_AddGroundTile(s, "herb", dirt, 0, i*TILESIZE, j*TILESIZE);
                break;
            case 'P' :
                scene_AddGroundTile(s, "herb", herb, 0, i*TILESIZE, j*TILESIZE);
                scene_AddCharacter(s, s->carDef.names[0], i*TILESIZE, j*TILESIZE);
                scene_setPlayer(s, &s->persos[s->nbPerso-1]);
                printf("Player id : %i\n", s->persos[s->nbPerso-1].gameObject.id);
                break;
            case '\n' :
                j++;
                i = -1;
                break;
            default:
                scene_AddGroundTile(s, "herb", herb, 0, i*TILESIZE, j*TILESIZE);
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
        strcpy(l->player->gameObject.name, "Player");
    }
    if(l->player->gameObject.id != l->idPlayer)
    {
        l->player = scene_getPersoById(l, l->idPlayer);
        strcpy(l->player->gameObject.name, "Player");
    }


    if(l->player->status != dead)
    {
        /*
        if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED) {
            moveToMouse(&l->persos[0].gameObject);
            gameObject_addLife(&l->persos[0].gameObject,10);
        }*/
        static SDL_bool leftPressed = SDL_FALSE;
        static SDL_bool rightPressed = SDL_FALSE;
        static SDL_bool middlePressed = SDL_FALSE;
        SDL_GetMouseState(&l->mousePos.x, &l->mousePos.y);
        l->mousePos.x -= l->offsetX;
        l->mousePos.y -= l->offsetY;
        SDL_Point pureMousePos;
        SDL_GetMouseState(&pureMousePos.x, &pureMousePos.y);
        l->focusTile = scene_getFocusTile(l->groundTiles, l->nbGroundTile, pureMousePos);
        l->interface.cursor.focusTile = l->focusTile;

        SDL_bool optionsSelected = SDL_FALSE;
        if(l->interface.optionActive)
        {
            for(int i = 0; i < l->interface.nbOptions; i++)
            {
                if(SDL_PointInRect(&pureMousePos, interface_getOptionBox(&l->interface, i)))
                {
                    l->interface.selection = i;
                    optionsSelected = SDL_TRUE;
                }
            }
        }

        if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED && leftPressed == SDL_FALSE)
        {
            leftPressed = SDL_TRUE;
            l->player->gameObject.direction = getAngleDirection(getRadAngle(l->player->gameObject.coord.y + l->player->gameObject.boxCollider.h /2
                                                                            - l->mousePos.y,
                                                                            l->mousePos.x
                                                                            - (l->player->gameObject.coord.x + l->player->gameObject.boxCollider.w /2)));
            if(l->interface.optionActive)
            {
                if(optionsSelected)
                {
                    interface_activerOption(&l->interface, l->player);
                }
                else
                {
                    interface_cacherOptions(&l->interface);
                }
            }
            else if(l->interface.cursor.active)
            {
                l->player->focusTile = cursor_desactivate(&l->interface.cursor);
                perso_setStatus(l->player, farm, NULL);
            }
            else
            {
                perso_setStatus(l->player, attack, NULL);
                for(int i = 0; i < l->nbPerso; i++)
                {
                    if(SDL_PointInRect(&l->mousePos, gameObject_getBoxCollider(&l->persos[i].gameObject)))
                    {
                        if(l->debugMode == SDL_TRUE)
                        {
                            printf("name: %s id : %i\n", l->persos[i].gameObject.name, l->persos[i].gameObject.id);
                            printf("life: %.1f/%.1f\n", l->persos[i].gameObject.life, l->persos[i].gameObject.maxLife);
                            printf("food: %.1f/%.1f\n", l->persos[i].gameObject.food, l->persos[i].gameObject.maxFood);
                            printf("stamina: %.1f/%.1f\n", l->persos[i].gameObject.stamina, l->persos[i].gameObject.maxStamina);
                            printf("strength: %.1f/%.1f (agi : %.2f)\n", l->persos[i].gameObject.strength, l->persos[i].gameObject.mass, l->persos[i].gameObject.agility);
                            printf("Vassal of ");
                            if(l->persos[i].liege != NULL)
                                printf("%s\n", l->persos[i].liege->gameObject.name);
                            else
                                printf("no one\n");

                            printf("Status : ");
                            switch(l->persos[i].status){
                            case idle: {printf("idle");break;}
                            case walk: {printf("walk");break;}
                            case farm: {printf("farm");break;}
                            case block: {printf("block");break;}
                            case attack: {printf("attack");break;}
                            case hit: {printf("hit");break;}
                            case dead: {printf("dead");break;}
                            }
                            printf("\n");

                            printf("Currently : ");
                            switch(l->persos[i].iaState){
                            case wandering: {printf("wandering");break;}
                            case fleeing: {printf("fleeing");break;}
                            case charging: {printf("charging %s(%i)", l->persos[i].pTarget->gameObject.name, l->persos[i].pTarget->gameObject.id);break;}
                            case fighting: {printf("fighting %s(%i)", l->persos[i].pTarget->gameObject.name, l->persos[i].pTarget->gameObject.id);break;}
                            case following: {printf("following %s(%i)", l->persos[i].pTarget->gameObject.name, l->persos[i].pTarget->gameObject.id);break;}
                            }
                            printf("\n");
                            switch(l->persos[i].gameObject.direction){
                            case north: {printf("direction: north \n");break;}
                            case south: {printf("direction: south \n");break;}
                            case west:  {printf("direction: west \n");break;}
                            case east:  {printf("direction: east \n");}break;}
                            if(l->persos[i].path != NULL)
                            {
                                vect2D pos = gameObject_getPos(l->persos[i].gameObject);
                                printf("Going to ");
                                if(l->persos[i].path->premier != NULL)
                                    printf("X%i Y%i.\nwhile being at X%i Y%i\n", l->persos[i].path->premier->nombre.x, l->persos[i].path->premier->nombre.y, pos.x, pos.y);
                                else
                                    printf("nowhere.\n");
                            }
                            printf("Neighbours : %i ( ", l->persos[i].nbNeighbours);
                            for(int j = 0; j < l->persos[i].nbNeighbours; j++)
                                printf("%i ", l->persos[i].neighbours[j]->gameObject.id);
                            printf(")\n");

                            printf("In sight : %i ( ", l->persos[i].nbInSightCharacters);
                            for(int j = 0; j < l->persos[i].nbInSightCharacters; j++)
                                printf("%i ", l->persos[i].inSightCharacters[j]->gameObject.id);
                            printf(")\n");


                            printf("Last in front : ");

                            if(l->persos[i].lastInFront != NULL)
                                printf("%i\n", l->persos[i].lastInFront->gameObject.id);
                            else
                                printf("no one\n");

                            printf("goal vx %f vy %f \n", l->persos[i].gameObject.goalV.x, l->persos[i].gameObject.goalV.y);
                            printf("speed vx %f vy %f \n", l->persos[i].gameObject.speed.x, l->persos[i].gameObject.speed.y);
                            if(l->persos[i].gameObject.passable == SDL_FALSE)
                                printf("not ");
                            printf("passable\n");
                            printf("\n");
                        }
                    }
                }
            }

        }
        else if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_RELEASED)
        {
            leftPressed = SDL_FALSE;
        }
        if(leftPressed)
        {

        }
        else if(rightPressed)
        {
            perso_setStatus(l->player, block, NULL);
            l->player->gameObject.direction = getAngleDirection(getRadAngle(l->player->gameObject.coord.y + l->player->gameObject.boxCollider.h /2
                                                                            - l->mousePos.y,
                                                                            l->mousePos.x
                                                                            - (l->player->gameObject.coord.x + l->player->gameObject.boxCollider.w /2)));

        }
        if(event.button.button == SDL_BUTTON_RIGHT && event.button.state == SDL_PRESSED && rightPressed == SDL_FALSE)
        {
            rightPressed = SDL_TRUE;
            int charFound = -1;
            for(int i = 0; i < l->nbPerso; i++)
            {
                if(SDL_PointInRect(&l->mousePos, gameObject_getBoxCollider(&l->persos[i].gameObject)))
                {
                    charFound = i;
                }
            }
            if(charFound != -1)
            {
                interface_afficherOptions(&l->interface, &l->persos[charFound], l->offsetX, l->offsetY);
            }
            else
            {
                interface_cacherOptions(&l->interface);
            }
        }
        else if(event.button.button == SDL_BUTTON_RIGHT && event.button.state == SDL_RELEASED)
        {
            perso_setStatus(l->player, idle, NULL);
            rightPressed = SDL_FALSE;
        }
        if(event.button.button == SDL_BUTTON_MIDDLE && event.button.state == SDL_PRESSED && middlePressed == SDL_FALSE)
        {
            scene_AddObject(l, "wall", (l->mousePos.x/TILESIZE) *TILESIZE, (l->mousePos.y/TILESIZE)*TILESIZE);
            ADD_inventoryItem(l->player->inventory, scene_findItem(l, "seed"), 3);
            middlePressed = SDL_TRUE;
        }
        else if(event.button.button == SDL_BUTTON_MIDDLE && event.button.state == SDL_RELEASED)
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

            if(TKEYPRESSED[SDL_SCANCODE_W] && l->player->status != attack)
            {
                perso_moveUp(l->player);
            }
            else if(TKEYPRESSED[SDL_SCANCODE_S] && l->player->status != attack)
            {
                perso_moveDown(l->player);
            }
            else
            {
                perso_slowY(l->player);
            }
            if(TKEYPRESSED[SDL_SCANCODE_A] && l->player->status != attack)
            {
                perso_moveLeft(l->player);
            }
            else if(TKEYPRESSED[SDL_SCANCODE_D] && l->player->status != attack)
            {
                perso_moveRight(l->player);
            }
            else
            {
                perso_slowX(l->player);
            }
            if (TKEYPRESSED[SDL_SCANCODE_F]) {
                interface_activateCursor(&l->interface);
            }
            if (TKEYPRESSED[SDL_SCANCODE_R]) {
            }


    }
}

groundTile * scene_getFocusTile(groundTile * groundTiles, int nbGroundTiles, SDL_Point mousePos)
{
    for(int i = 0; i < nbGroundTiles; i++)
    {
        if(SDL_PointInRect(&mousePos, &groundTiles[i].sprites[groundTiles[i].groundState].imgPos))
        {
            return &groundTiles[i];
        }
    }
    return NULL;
}

void scene_DRAW(scene * lescene, SDL_Renderer * renderer, SDL_Window * window)
{
    Uint32 ticks = SDL_GetTicks();
    while(ticks - lescene->timeSinceLastAction < lescene->timeBetweenAction)
    {
        ticks = SDL_GetTicks();
    }
    lescene->timeSinceLastAction = ticks;
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


    for(int i = 0; i < lescene->nbGroundTile; i++)
        groundTile_DRAW(&lescene->groundTiles[i], renderer, lescene->offsetX, lescene->offsetY);
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
        if(lescene->persos[i].status == attack && sprite_getFrame(perso_getSprite(&lescene->persos[i], attack)) == lescene->persos[i].attackFrame )
        {
            for(int j = 0; j < lescene->nbPerso; j++)
            {
                if(i == j)
                {
                    continue;
                }

                if(SDL_HasIntersection(perso_getAttackBox(&lescene->persos[i]), gameObject_getBoxCollider(&lescene->persos[j].gameObject)))
                {
                    if((lescene->persos[j].status == block) && lescene->persos[j].gameObject.direction != lescene->persos[i].gameObject.direction)
                    {
                        perso_setStatus(&lescene->persos[i], hit, NULL);
                        perso_blockHit(&lescene->persos[j], lescene->persos[i].weapon->damage);
                    }
                    else
                    {
                        perso_setStatus(&lescene->persos[j], hit, &lescene->persos[i]);
                    }
                }
            }
        }
        if(lescene->persos[i].status == farm && lescene->persos[i].sprites[farm].frame > 2 && perso_finishedAction(&lescene->persos[i]) && perso_focusTileIsInRange(lescene->persos[i]))
        {
            groundTile * focusTile = perso_getFocusTile(lescene->persos[i]);
            if(focusTile != NULL)
            {
                int x = focusTile->coord.x/TILESIZE;
                int y = focusTile->coord.y/TILESIZE;

                ADD_inventoryItem(lescene->persos[i].inventory, scene_findItem(lescene, "seed"), -1);
                groundTile_changeState(groundTile_getTileFromPos(lescene->groundTiles, lescene->nbGroundTile, x , y), seed);
                groundTile * gt1 = groundTile_getTileFromPos(lescene->groundTiles, lescene->nbGroundTile, x +1, y);
                groundTile * gt2 = groundTile_getTileFromPos(lescene->groundTiles, lescene->nbGroundTile, x -1, y);
                groundTile * gt3 = groundTile_getTileFromPos(lescene->groundTiles, lescene->nbGroundTile, x, y-1);
                groundTile * gt4 = groundTile_getTileFromPos(lescene->groundTiles, lescene->nbGroundTile, x, y+1);
                if(gt1->groundState != dirt && !groundTile_IsField(*gt1))
                    groundTile_changeState(gt1, dirt);
                if(gt2->groundState != dirt && !groundTile_IsField(*gt2))
                    groundTile_changeState(gt2, dirt);
                if(gt3->groundState != dirt && !groundTile_IsField(*gt3))
                    groundTile_changeState(gt3, dirt);
                if(gt4->groundState != dirt && !groundTile_IsField(*gt4))
                    groundTile_changeState(gt4, dirt);
            }
        }

        perso_getFriends(&lescene->persos[i], lescene->persos, lescene->nbPerso);

        perso_Simulate(&lescene->persos[i], lescene->objects, lescene->nbObject, ticks);

    }

    /***************************************
    VERIFIE LES COLLISIONS ENTRE LES PERSONNAGES
    ***************************************/
    for (int i = 0; i < lescene->nbPerso; i++)
    {
        for(int j = 0; j < lescene->nbPerso ; j++)
        {
            if(i == j || lescene->persos[i].gameObject.passable == SDL_TRUE || lescene->persos[j].gameObject.passable == SDL_TRUE)
            {
                continue;
            }
            while(getDistanceBetween(lescene->persos[i].gameObject, lescene->persos[j].gameObject) < lescene->persos[i].gameObject.boxCollider.w && lescene->persos[i].status != dead && lescene->persos[j].status != dead)
            {
                // printf("Oui %i %i X1 : %f Y1 : %f, X2 : %f Y2 : %f / %f\n", lescene->persos[i].gameObject.id, lescene->persos[j].gameObject.id, lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y, lescene->persos[j].gameObject.coord.x, lescene->persos[j].gameObject.coord.y , ((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->persos[j].gameObject.coord.y + lescene->persos[j].gameObject.boxCollider.h/2)));
                if(((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->persos[j].gameObject.coord.y + lescene->persos[j].gameObject.boxCollider.h/2)) != 0.0)
                {

                    float direc1 = lescene->persos[i].gameObject.boxCollider.w;
                    float direc2 = lescene->persos[j].gameObject.boxCollider.w;
                    float direc3 = lescene->persos[i].gameObject.boxCollider.h;
                    float direc4 = lescene->persos[j].gameObject.boxCollider.h;

                    float angle = fabs(getRadAngle(((lescene->persos[i].gameObject.coord.x + direc1 / 2.0)
                                             -  (lescene->persos[j].gameObject.coord.x + direc2 / 2.0)),
                                               ((lescene->persos[i].gameObject.coord.y + direc3 / 2.0)
                                             -  (lescene->persos[j].gameObject.coord.y + direc4 / 2.0))));

                    float rat1 = lescene->persos[i].gameObject.boxCollider.h/2.0;
                    float rat2 = lescene->persos[i].gameObject.boxCollider.w/2.0;
                    float maxAngle = fabs(getRadAngle(rat1, rat2));

                    if(angle >= maxAngle && angle <= 180-maxAngle)
                    {
                        if(((lescene->persos[i].gameObject.coord.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->persos[j].gameObject.coord.x + lescene->persos[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x - 1, lescene->persos[j].gameObject.coord.y);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                            gameObject_slow(&lescene->persos[j].gameObject.speed.x, lescene->persos[j].gameObject.a);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x + 1, lescene->persos[j].gameObject.coord.y);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                            gameObject_slow(&lescene->persos[j].gameObject.speed.x, lescene->persos[j].gameObject.a);
                        }

                    }
                    else
                    {
                        if(((lescene->persos[i].gameObject.coord.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->persos[j].gameObject.coord.y + lescene->persos[j].gameObject.boxCollider.h/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y + 1);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x, lescene->persos[j].gameObject.coord.y - 1);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                            gameObject_slow(&lescene->persos[j].gameObject.speed.y, lescene->persos[j].gameObject.a);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y - 1);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x, lescene->persos[j].gameObject.coord.y + 1);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                            gameObject_slow(&lescene->persos[j].gameObject.speed.y, lescene->persos[j].gameObject.a);
                        }
                    }
                }
                else
                {
                        if(((lescene->persos[i].gameObject.coord.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->persos[j].gameObject.coord.x + lescene->persos[j].gameObject.boxCollider.w/2)) > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x - 1, lescene->persos[j].gameObject.coord.y);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                            gameObject_slow(&lescene->persos[j].gameObject.speed.x, lescene->persos[j].gameObject.a);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                            perso_MOVETO(&lescene->persos[j], lescene->persos[j].gameObject.coord.x + 1, lescene->persos[j].gameObject.coord.y);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                            gameObject_slow(&lescene->persos[j].gameObject.speed.x, lescene->persos[j].gameObject.a);
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
                    float rat = atan( fabs( rat1 / rat2) )*2/PI;
                    int diffX = ((lescene->persos[i].gameObject.boxCollider.x + lescene->persos[i].gameObject.boxCollider.w/2) - (lescene->objects[j].gameObject.boxCollider.x + lescene->objects[j].gameObject.boxCollider.w/2));
                    int diffY = ((lescene->persos[i].gameObject.boxCollider.y + lescene->persos[i].gameObject.boxCollider.h/2) - (lescene->objects[j].gameObject.boxCollider.y + lescene->objects[j].gameObject.boxCollider.h/2));
                    if(direc > rat)
                    {
                        if(diffX > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                        }
                    }
                    else if(direc < rat)
                    {
                        if(diffY > 0)
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y + 1);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                        }
                        else
                        {
                            perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y - 1);
                            gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                        }
                    }
                    else
                    {
                        SDL_Rect hitboxX = lescene->persos[i].gameObject.boxCollider;
                        if(diffX > 0)
                        {
                            hitboxX.x++;
                        }
                        else
                        {
                            hitboxX.x--;
                        }
                        SDL_Rect hitboxY = lescene->persos[i].gameObject.boxCollider;
                        if(diffY > 0)
                        {
                            hitboxY.y++;
                        }
                        else
                        {
                            hitboxY.y--;
                        }
                        if(!SDL_HasIntersection(&hitboxX, gameObject_getBoxCollider(&lescene->objects[j].gameObject)))
                        {
                            if(diffX >= 0)
                            {
                                perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                                gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                            }
                            else
                            {
                                perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                                gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                            }
                        }
                        else if(!SDL_HasIntersection(&hitboxY, gameObject_getBoxCollider(&lescene->objects[j].gameObject)))
                        {
                            if(diffY >= 0)
                            {
                                perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y + 1);
                                gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                            }
                            else
                            {
                                perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y - 1);
                                gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                            }
                        }
                        else
                        {
                            if(rand()%2)
                            {

                                if(diffX > 0)
                                {
                                    perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x + 1, lescene->persos[i].gameObject.coord.y);
                                    gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                                }
                                else
                                {
                                    perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x - 1, lescene->persos[i].gameObject.coord.y);
                                    gameObject_slow(&lescene->persos[i].gameObject.speed.x, lescene->persos[i].gameObject.a);
                                }
                            }
                            else
                            {

                                if(diffY > 0)
                                {
                                    perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y + 1);
                                    gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                                }
                                else
                                {
                                    perso_MOVETO(&lescene->persos[i], lescene->persos[i].gameObject.coord.x, lescene->persos[i].gameObject.coord.y - 1);
                                    gameObject_slow(&lescene->persos[i].gameObject.speed.y, lescene->persos[i].gameObject.a);
                                }
                            }
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
    free(lescene->items);
    free(lescene->persos);
    free(lescene->objects);
    free(lescene->player);
}
