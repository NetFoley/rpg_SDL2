#include "perso.h"

void perso_INI(perso * p, void * scene, SDL_Renderer * renderer, perso * model, int x, int y)
{
    if(model == NULL || p == NULL)
    {
        printf("INI Character : echec\n");
        exit(0);
    }
    //COPY FROM MODEL
    p->weapon = (item*)malloc(sizeof(item*));
    p->pTarget = (perso*)malloc(sizeof(perso*));
    p->lastInFront = (perso*)malloc(sizeof(perso*));
	p->neighbours = (perso**)malloc(sizeof(perso*));
	p->inSightCharacters = (perso**)malloc(sizeof(perso*));
	p->focusTile = NULL;
    *p = *model;
    p->scene = scene;
    p->pTarget = NULL;
    p->lastInFront = NULL;
	p->neighbours = NULL;
    p->inventory = INIT_Inventory();

	p->inSightCharacters = NULL;
	p->nbNeighbours = 0;
	float random1 = (0.8 + (rand()%4)/10.0);
	float random2 = (0.8 + (rand()%4)/10.0);
    gameObject_INI(&p->gameObject, model->gameObject.name, model->gameObject.mass * random1, model->gameObject.strength * random2, x, y, model->gameObject.passable);
    perso_updateAgilities(p);
    p->path = initialiser();
	p->inFront = SDL_FALSE;

    /*
    for(int i = 0; i < dead + 1; i++)
        p->sprites[i] = model->sprites[i];

    p->race = model->race;
    for(int i = 0; i < undead + 1; i++)
        p->raceAttitude[i] = model->raceAttitude[i];

    p->attackFrame = model->attackFrame;
    p->status = idle;
    p->following = 0;
    p->gameObject.life = p->gameObject.maxLife;
    p->hitRange = model->hitRange;
    strcpy(p->name, model->name);
    p->path = initialiser();
    p->playerControled = 0;
    p->range = model->range;
    p->weapon = model->weapon;
    &p->pTarget->gameObject = NULL;
    p->attackBoxOffset = model->attackBoxOffset;
    */

}

void newModel_INI(perso * lePerso, char * name, float mass, float str, SDL_bool passable, int race, int attackFrame, int range, int hitRange, sprite sprites[dead + 1], item * w, int intelligence)
{
    gameObject_INI(&lePerso->gameObject, name, mass, str, 0, 0, passable);
    lePerso->race = race;
    lePerso->attackFrame = attackFrame;
    lePerso->weapon = (item*)malloc(sizeof(item*));
    perso_equipWeapon(lePerso, w);
    lePerso->pTarget = (perso*)malloc(sizeof(perso*));
    lePerso->pTarget = NULL;
    lePerso->neighbours = (perso**)malloc(sizeof(perso*));
    lePerso->inSightCharacters = (perso**)malloc(sizeof(perso*));
    lePerso->neighbours = NULL;
    lePerso->inSightCharacters = NULL;
    lePerso->iaState = wandering;
	lePerso->neighboursDistance = 145; //130
	lePerso->avoidDistance = lePerso->gameObject.boxCollider.w*1.3; //92
	lePerso->liege = NULL;
	lePerso->behavior.intelligence = intelligence;

	for(int i = 0; i < undead+1; i++)
	{
		if(lePerso->race == i)
		{
			lePerso->raceAttitude[i] = 10;
		}
		else
		{
			lePerso->raceAttitude[i] = -10;
		}
	}

    for(int i = 0; i < dead + 1 ; i++)
        lePerso->sprites[i] = sprites[i];

    lePerso->sprites[dead].repeat = SDL_FALSE;
    lePerso->sprites[block].repeat = SDL_FALSE;
    lePerso->sprites[farm].repeat = SDL_FALSE;
    lePerso->sprites[attack].repeat = SDL_FALSE;
    lePerso->sprites[hit].repeat = SDL_FALSE;

    for(int i = 0; i < dead + 1; i++)
    {
        lePerso->shadowSprites[i] = lePerso->sprites[i];
        lePerso->shadowSprites[i].imgPos.w = lePerso->sprites[i].imgPos.w;
        lePerso->shadowSprites[i].imgPos.h = lePerso->sprites[i].imgPos.h / 4;
    }

    lePerso->attackBoxOffset.x = sprite_getWidth(perso_getSprite(lePerso, walk));
    lePerso->attackBoxOffset.y = sprite_getHeight(perso_getSprite(lePerso, walk))/2;
    lePerso->attackFrame = attackFrame;
    lePerso->sightRange = range;
    lePerso->hitRange = hitRange;

    lePerso->gameObject.boxColliderOffset.x = 0;
    lePerso->gameObject.boxColliderOffset.y = sprite_getHeight(lePerso->sprites[walk]) - lePerso->gameObject.boxCollider.h;
    gameObject_getBoxCollider(&lePerso->gameObject);

    lePerso->status = idle;
    lePerso->playerControled = SDL_FALSE;
    lePerso->path = initialiser();
    printf("Initialised character (%s) mass(%.1f) strength(%.1f)\n life(%.1f) speed(%f) acc(%.1f)\n",
           lePerso->gameObject.name, lePerso->gameObject.mass, lePerso->gameObject.strength, lePerso->gameObject.maxLife, lePerso->gameObject.maxS, lePerso->gameObject.a);
}

sprite * model_getSprites(perso * model)
{
    return model->sprites;
}

void perso_equipWeapon(perso * p, item * w)
{
    if(w->itemType == weapon)
    {
        printf("Equipped %s with %s\n", p->gameObject.name, w->name);
        p->weapon = w;
    }
    else
    {
        printf("Error can't equip %s\n", w->name);
    }
}

SDL_bool perso_isLiege(perso * p, perso * l)
{
    SDL_bool isLiege = SDL_FALSE;
    perso * target = p->liege;
    while(target != NULL && isLiege == SDL_FALSE)
    {
        if(target == l)
            isLiege = SDL_TRUE;
        target = target->liege;
    }
    return isLiege;
}

SDL_bool isIAStateAgressive(perso * p)
{
    SDL_bool iaState = SDL_FALSE;
    if(p->iaState == charging || p->iaState == fighting)
        iaState = SDL_TRUE;

    return iaState;
}

int perso_makeFollow(perso * p, perso * l)
{
    if(perso_isLiege(p, l) || perso_getAttitude(p, l) > 0)
    {
        p->iaState = following;
        perso_setTarget(p, l);
        return 1;
    }
    return 0;
}

int perso_makeFollowAll(perso * l, perso * p)
{
    for(int i = 0; i < p->nbInSightCharacters; i++)
       perso_makeFollow(p->inSightCharacters[i], p);
    return 0;
}

int perso_makeFlee(perso * p)
{
    p->iaState = fleeing;
    perso_setTarget(p, NULL);
    return 0;
}

float perso_getPower(perso * p)
{
    return p->gameObject.mass + gameObject_getSTR(&p->gameObject);
}

float perso_getAverageInSightPower(perso * p)
{
    float allyPower = perso_getPower(p);
    float ennemiPower = 0;
    for(int i = 0; i < p->nbInSightCharacters; i++)
    {
        if(perso_isAlive(*p->inSightCharacters[i]) && perso_getAttitude(p, p->inSightCharacters[i]) >= 0)
        {
            allyPower += perso_getPower(p->inSightCharacters[i]);
        }
        else
        {
            ennemiPower += perso_getPower(p->inSightCharacters[i]);
        }
    }
    return ennemiPower/allyPower;

}

float perso_getBravery(perso * p)
{
    //p->behavior...
    return 2;
}

int perso_makeAttack(perso * p, perso * l)
{
    p->iaState = charging;
    perso_setTarget(p, l);
    return 0;
}

int perso_makeWander(perso * p)
{
    p->iaState = wandering;
    perso_setTarget(p, NULL);
    return 0;
}

int perso_getAttitude(perso*p, perso *l)
{
    int attitude = perso_getRaceAttitude(p, l->race);
    if(perso_isLiege(p, l))
       attitude=+ 10;
    return attitude;
}

int perso_getRaceAttitude(perso * p, race r)
{
    return p->raceAttitude[r];
}

int getDetecRange(perso p)
{
    return p.sightRange;
}

SDL_Rect * perso_getAttackBox(perso * p)
{
    if(p->gameObject.direction == east)
    {
        p->weapon->hitBox.x = p->gameObject.coord.x + p->gameObject.boxCollider.w;
        p->weapon->hitBox.y = p->gameObject.coord.y + p->gameObject.boxCollider.h/2 - p->weapon->hitBox.h/2;
    }
    else if(p->gameObject.direction == west)
    {
        p->weapon->hitBox.x = p->gameObject.coord.x - p->weapon->hitBox.w;
        p->weapon->hitBox.y = p->gameObject.coord.y + p->gameObject.boxCollider.h/2 - p->weapon->hitBox.h/2;
    }
    else if(p->gameObject.direction == north)
    {
        p->weapon->hitBox.x = p->gameObject.coord.x + p->gameObject.boxCollider.w/2 - p->weapon->hitBox.w/2;
        p->weapon->hitBox.y = p->gameObject.coord.y - p->weapon->hitBox.h;
    }
    else if(p->gameObject.direction == south)
    {
        p->weapon->hitBox.x = p->gameObject.coord.x + p->gameObject.boxCollider.w/2 - p->weapon->hitBox.w/2;
        p->weapon->hitBox.y = p->gameObject.coord.y + p->gameObject.boxCollider.h;
    }
    return &p->weapon->hitBox;
}

void perso_setStatus(struct perso * p, status status, perso * p2)
{
    if(p->status != dead && (p->status != status || status == hit) && (status == dead || status == hit || p->status != hit || sprite_animationEnded(&p->sprites[hit])))
    {
        sprite_RESET(&p->sprites[p->status]);
        sprite_RESET(&p->shadowSprites[p->status]);
        switch(status)
        {
        case hit: ;
            SDL_bool gotHit = SDL_FALSE;
            if(p2 != NULL)
            {
                gotHit = perso_getHit(p, -p2->weapon->damage - (p2->gameObject.strength / 10.0) * gameObject_updateAgility(&p->gameObject));
                if(gotHit)
                {
                    float angle = perso_getRadAngle(*p2, *p);
                    fvect2D posRatio = getPosRatioFromAngle(angle);
                    posRatio.x *= 5 * gameObject_getPower(&p2->gameObject) / gameObject_getPower(&p->gameObject);
                    posRatio.y *= 5 * gameObject_getPower(&p2->gameObject) / gameObject_getPower(&p->gameObject);
                    gameObject_addSpeed(&p->gameObject, posRatio);
                }

            }
            else
            {
                gotHit = perso_getHit(p, 0);
            }
            if(gotHit)
            {
                p->status = status;
                sprite_RESET(&p->sprites[status]);
                sprite_RESET(&p->shadowSprites[status]);
            }

            //printf("ID(%i) suffer %i damage(%i/%i) from %s\n", p->gameObject.id, p2->weapon->damage, p->gameObject.life, p->gameObject.maxLife, p2->weapon->name);
            if(p2 != NULL && p->playerControled == SDL_FALSE)
            {
                perso_makeAttack(p, p2);
                //printf("ID(%i) targetting ID(%i)\n\n", p->gameObject.id, p2->gameObject.id);
            }
        break;

        case block:
            if(p->gameObject.stamina > 0)
            {
                p->status = block;
                sprite_RESET(&p->sprites[block]);
                sprite_RESET(&p->shadowSprites[block]);
            }
            break;

        case farm:
            if(perso_focusTileIsInRange(*p) && ADD_inventoryItem(p->inventory, scene_findItem(p->scene, "seed"), -1))
            {
                ADD_inventoryItem(p->inventory, scene_findItem(p->scene, "seed"), 1);
                p->status = farm;
                sprite_RESET(&p->sprites[farm]);
                sprite_RESET(&p->shadowSprites[farm]);
            }
            break;

        default:
            p->status = status;
            sprite_RESET(&p->sprites[status]);
            sprite_RESET(&p->shadowSprites[status]);
        }

    }
}

void perso_addLife(perso * p, float life)
{
    gameObject_addLife(&p->gameObject, life);
    perso_updateAgilities(p);
}

SDL_bool perso_getHit(perso * p, float life)
{
    perso_updateAgilities(p);
    return gameObject_getHit(&p->gameObject, life);
}

void perso_updateAgilities(perso * p)
{
    for(int i = 0; i < dead; i++)
    {
        sprite_setTotalTime(&p->sprites[i], 50 + 500/(0.15+p->gameObject.agility));
        sprite_setTotalTime(&p->shadowSprites[i], 50 + 500/(0.15+p->gameObject.agility));
    }
    if(p->gameObject.agility <= 0.1)
    {
        perso_setStatus(p, dead, NULL); // UNCONCIOUS
    }
}

groundTile * perso_getFocusTile(perso p)
{
    return p.focusTile;
}

gameObject * perso_getGameObject(perso * p)
{
    return &p->gameObject;
}

SDL_bool perso_focusTileIsInRange(perso p)
{
    if(getDistfVect2D(p.gameObject.coord, p.focusTile->coord) < 2*TILESIZE)
    {
        return SDL_TRUE;
    }
    printf("Focused Tile NOT in range ! Character name : %s ID : %i\n", p.gameObject.name, p.gameObject.id);
    return SDL_FALSE;
}

void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY, SDL_bool debugOn)
{
    SDL_SetTextureColorMod(lePerso->shadowSprites[lePerso->status].texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(lePerso->shadowSprites[lePerso->status].texture, 200);

    sprite_DRAW(&lePerso->shadowSprites[lePerso->status], renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY);
    SDL_SetTextureColorMod(lePerso->shadowSprites[lePerso->status].texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(lePerso->shadowSprites[lePerso->status].texture, 255);
    if(lePerso->status == walk)
        sprite_setTotalTime(&lePerso->sprites[walk], 300*lePerso->gameObject.stance);
    if(lePerso->gameObject.direction == east)
    {
        for(int i = 0; i < dead; i++)
        {
            sprite_FLIP_RIGHT(&lePerso->sprites[i]);
            sprite_FLIP_RIGHT(&lePerso->shadowSprites[i]);
        }
        sprite_DRAW(&lePerso->sprites[lePerso->status], renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY);
    }

    else if(lePerso->gameObject.direction == west)
    {
        for(int i = 0; i < dead; i++)
        {
            sprite_FLIP_LEFT(&lePerso->sprites[i]);
            sprite_FLIP_LEFT(&lePerso->shadowSprites[i]);
        }
        sprite_DRAW(&lePerso->sprites[lePerso->status], renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY);
    }
    else if(lePerso->gameObject.direction == north)
    {
        sprite_DRAW(&lePerso->sprites[lePerso->status], renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY);
    }
    else if(lePerso->gameObject.direction == south)
    {
        sprite_DRAW(&lePerso->sprites[lePerso->status], renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY);
    }


    if(debugOn)
    {

        static SDL_Texture* text1 = NULL;
        static SDL_Texture* text2 = NULL;

        SDL_Rect box1;
        box1 = lePerso->weapon->hitBox;
        box1.x += offsetX;
        box1.y += offsetY;
        SDL_Rect box2;
        box2 = lePerso->gameObject.boxCollider;
        box2.x += offsetX;
        box2.y += offsetY;

        if(text1 == NULL)
            text1 = ini_Box(lePerso->weapon->hitBox.w, lePerso->weapon->hitBox.h, 150, 50, 50, renderer);
        if(text2 == NULL)
            text2 = ini_Box(lePerso->gameObject.boxCollider.w, lePerso->gameObject.boxCollider.h, 50, 150, 50, renderer);
        SDL_RenderCopy(renderer, text1, NULL, &box1);
        SDL_RenderCopy(renderer, text2, NULL, &box2);
    }

}

void perso_MOVETO(struct perso * lePerso, int posX, int posY)
{
    lePerso->gameObject.coord.x = posX;
    lePerso->gameObject.coord.y = posY;
}

void perso_setTarget(perso * lePerso, perso * target)
{
    if(lePerso->playerControled == SDL_FALSE && lePerso->pTarget != target)
    {
        lePerso->pTarget = target;
        viderFile(lePerso->path);
        if(lePerso->status == block)
            perso_setStatus(lePerso, idle, NULL);
    }
}


SDL_bool line_isFree(perso p, object * objects, int nbObjects)
{
    line line1;
    line1.p1.x = p.gameObject.coord.x;
    line1.p1.y = p.gameObject.coord.y;
    line1.p2.x = p.path->premier->suivant->nombre.x*TILESIZE;
    line1.p2.y = p.path->premier->suivant->nombre.y*TILESIZE;
    line line2;
    line2.p1.x = p.gameObject.coord.x + p.gameObject.boxCollider.w-1;
    line2.p1.y = p.gameObject.coord.y;
    line2.p2.x = p.path->premier->suivant->nombre.x*TILESIZE + TILESIZE-1;
    line2.p2.y = p.path->premier->suivant->nombre.y*TILESIZE;
    line line3;
    line3.p1.x = p.gameObject.coord.x;
    line3.p1.y = p.gameObject.coord.y + p.gameObject.boxCollider.h-1;
    line3.p2.x = p.path->premier->suivant->nombre.x*TILESIZE;
    line3.p2.y = p.path->premier->suivant->nombre.y*TILESIZE + TILESIZE-1;
    line line4;
    line4.p1.x = p.gameObject.coord.x + p.gameObject.boxCollider.w-1;
    line4.p1.y = p.gameObject.coord.y + p.gameObject.boxCollider.h-1;
    line4.p2.x = p.path->premier->suivant->nombre.x*TILESIZE + TILESIZE-1;
    line4.p2.y = p.path->premier->suivant->nombre.y*TILESIZE + TILESIZE-1;
    SDL_bool free = SDL_TRUE;

    for(int i = 0; i < nbObjects; i++)
    {
        if(SDL_IntersectRectAndLine(gameObject_getBoxCollider(&objects[i].gameObject), &line1.p1.x, &line1.p1.y, &line1.p2.x, &line1.p2.y))
            free = SDL_FALSE;
        if(SDL_IntersectRectAndLine(gameObject_getBoxCollider(&objects[i].gameObject), &line2.p1.x, &line2.p1.y, &line2.p2.x, &line2.p2.y))
            free = SDL_FALSE;
        if(SDL_IntersectRectAndLine(gameObject_getBoxCollider(&objects[i].gameObject), &line3.p1.x, &line3.p1.y, &line3.p2.x, &line3.p2.y))
            free = SDL_FALSE;
        if(SDL_IntersectRectAndLine(gameObject_getBoxCollider(&objects[i].gameObject), &line4.p1.x, &line4.p1.y, &line4.p2.x, &line4.p2.y))
            free = SDL_FALSE;
    }

    return free;
}

void perso_Simulate(perso * lePerso, object * objects, int nbObject, Uint32 ticks)
{
    if(lePerso->status != dead)
    {
        /*****************************
                    STATS
        *****************************/
        gameObject_getBoxCollider(&lePerso->gameObject);
        if(gameObject_getLife(lePerso->gameObject) <= 0)
        {
            perso_setStatus(lePerso, dead, NULL);
            lePerso->gameObject.passable = SDL_TRUE;
        }
        if(!perso_isMaxLife(*lePerso) && gameObject_getFood(lePerso->gameObject) > lePerso->gameObject.maxFood/4)
        {
            float dedux = gameObject_getStamina(lePerso->gameObject)/20000;
            gameObject_addStamina(&lePerso->gameObject, -dedux);
            perso_addLife(lePerso, dedux);
        }
        if(gameObject_getFood(lePerso->gameObject) <= 0)
        {
            gameObject_addFood(&lePerso->gameObject, 2);
            perso_addLife(lePerso, -1);
            gameObject_addSTR(&lePerso->gameObject, -gameObject_getSTR(&lePerso->gameObject)/100);
        }
        if(lePerso->status == block)
        {
            gameObject_addStamina(&lePerso->gameObject, -0.1);
        }
        if(!perso_isMaxStamina(*lePerso))
        {
            gameObject_addStamina(&lePerso->gameObject, 0.3);
            gameObject_addFood(&lePerso->gameObject, -0.001);
        }
        gameObject_addFood(&lePerso->gameObject, -0.001);

        if(lePerso->playerControled == SDL_FALSE)
        {
            /*****************************
                  DECIDING BEVAHIOR
            *****************************/
            perso * newTarget = NULL;
            //Check aggro target
            for(int j = 0; j < lePerso->nbInSightCharacters; j++)
            {
                if(perso_isAlive(*lePerso->inSightCharacters[j])
                    && perso_getAttitude(lePerso, lePerso->inSightCharacters[j]) < 0
                   && (newTarget == NULL || getDistanceBetween(lePerso->gameObject, newTarget->gameObject)
                       > getDistanceBetween(lePerso->gameObject, lePerso->inSightCharacters[j]->gameObject)))
                    {
                        newTarget = lePerso->inSightCharacters[j];
                    }
            }

            if(lePerso->iaState != following)
            {
            if(newTarget == NULL)
            {
                perso_makeWander(lePerso);
            }
            else if(perso_getAverageInSightPower(lePerso) < perso_getBravery(lePerso))
            {
                perso_makeAttack(lePerso, newTarget);
            }
            else
            {
                perso_makeFlee(lePerso);
            }

            }

            /**************************
                      ACTION
            **************************/
            fvect2D fCible;
            fCible.x = lePerso->gameObject.boxCollider.x;
            fCible.y = lePerso->gameObject.boxCollider.y;
            gameObject_getBoxCollider(&lePerso->gameObject);
                lePerso->inFront = perso_friendInFront(lePerso);
                switch(lePerso->iaState)
                {
                case (charging):
                    if(getDistanceBetween(lePerso->gameObject, lePerso->pTarget->gameObject) < lePerso->neighboursDistance)
                        perso_lookAt(lePerso, lePerso->pTarget);
                    if(!lePerso->inFront || lePerso->lastInFront == lePerso->pTarget)
                        fCible = perso_moveToTarget(lePerso, objects, nbObject);
                    if(lePerso->status == walk || lePerso->status == idle)
                    {
                        if(lePerso->weapon->weaponType == range)
                        {
                            if(getDistanceBetween(lePerso->gameObject, lePerso->pTarget->gameObject) < perso_getHitRange(lePerso))
                                perso_setStatus(lePerso, attack, NULL);
                        }
                        else
                        {
                            if(SDL_HasIntersection(perso_getAttackBox(lePerso), gameObject_getBoxCollider(&lePerso->pTarget->gameObject)))
                            {
                                perso_setStatus(lePerso, attack, NULL);
                            }
                        }
                    }
                    break;

                case(following):
                    if(getDistanceBetween(lePerso->gameObject, lePerso->pTarget->gameObject) < lePerso->neighboursDistance)
                        perso_lookAt(lePerso, lePerso->pTarget);
                    if(!lePerso->inFront)
                        fCible = perso_moveToTarget(lePerso, objects, nbObject);
                    break;
                case(fighting):
                    perso_lookAt(lePerso, lePerso->pTarget);
                    if(!lePerso->inFront || lePerso->lastInFront == lePerso->pTarget)
                        fCible = perso_moveToTarget(lePerso, objects, nbObject);
                    if(lePerso->status == walk || lePerso->status == idle)
                    {
                        if(lePerso->weapon->weaponType == range)
                        {
                            if(getDistanceBetween(lePerso->gameObject, lePerso->pTarget->gameObject) < perso_getHitRange(lePerso))
                                perso_setStatus(lePerso, attack, NULL);
                        }
                        else
                        {
                            if(SDL_HasIntersection(perso_getAttackBox(lePerso), gameObject_getBoxCollider(&lePerso->pTarget->gameObject)))
                            {
                                perso_setStatus(lePerso, attack, NULL);
                            }
                        }
                        if(lePerso->gameObject.stamina > 0 && perso_isGettingAttacked(lePerso))
                            perso_setStatus(lePerso, block, NULL);
                    }
                    if(lePerso->status == block && (perso_EnemyAroundHit(lePerso)))
                        perso_setStatus(lePerso, idle, NULL);

                    //BLOCK
                    break;

                case(fleeing): ;
                        vect2D nCible = findPosFromAngleDist(perso_getAngleAwayFromEnnemies(lePerso), 1);
                        vect2D nPos = gameObject_getPos(lePerso->gameObject);
                        nCible.x += nPos.x;
                        nCible.y += nPos.y;
                        fCible = perso_moveToPos(lePerso, nCible, objects, nbObject);
                    break;

                default:
                    break;
                }

            fvect2D goalV;
            //MOVING THE OBJECT
            goalV.x =  (float)fCible.x - lePerso->gameObject.boxCollider.x;
            goalV.y =  (float)fCible.y - lePerso->gameObject.boxCollider.y;
            //MOVING ACCORDING TO PATH

            setGoalV(&lePerso->gameObject, goalV);
            fvect2D avoidV = neighbour_getAvoidDir(lePerso, lePerso->neighbours, lePerso->nbNeighbours);
            if(lePerso->status != idle || getSpeedV(avoidV) > lePerso->gameObject.maxS/5)
                addGoalV(&lePerso->gameObject, avoidV);
            // printf("ACC :%f %f\n", o->speed.y, o->a);
        }
            if(lePerso->lastInFront != NULL)
            {
                if(lePerso->iaState != charging && perso_TargetInFront(lePerso) && getDistanceBetween(lePerso->gameObject, lePerso->lastInFront->gameObject) < lePerso->neighboursDistance * 1.5)
                    perso_changeStance(lePerso, walking);
                else
                    perso_changeStance(lePerso, running);
            }
            else
            {
                perso_changeStance(lePerso, running);
            }


            if(perso_finishedAction(lePerso))
            {
                perso_setStatus(lePerso, idle, NULL);
                if(lePerso->iaState == charging && lePerso->behavior.intelligence > 25)
                {
                    lePerso->iaState = fighting;
                }
            }

            if(!perso_canMove(lePerso))
            {
                fvect2D goalV;
                goalV.x =  0;
                goalV.y =  0;
                setGoalV(&lePerso->gameObject, goalV);
            }

            gameObject_addStamina(&lePerso->gameObject, -gameObject_simulate(&lePerso->gameObject) / 100);

            if(fabs(lePerso->gameObject.speed.x) < 0.1 && fabs(lePerso->gameObject.speed.y) < 0.1)
            {
                if(lePerso->status == walk)
                {
                    perso_setStatus(lePerso, idle, NULL);
                }
            }
            else
            {
                if(lePerso->status == idle)
                {
                    perso_setStatus(lePerso, walk, NULL);
                }
            }            //lePerso->gameObject.timeSinceLastMove = ticks;
    }
}

int perso_getAngleAwayFromEnnemies(perso * p)
{
    vect2D pPos = gameObject_getPos(p->gameObject);
    vect2D nAveragePoint;
    nAveragePoint.x = 0;
    nAveragePoint.y = 0;

    int i;
    for(i = 0; i < p->nbInSightCharacters; i++)
    {
        if(perso_getAttitude(p, p->inSightCharacters[i]) < 0)
        {
            vect2D nSight = gameObject_getPos(p->inSightCharacters[i]->gameObject);
            nAveragePoint.x += pPos.x - nSight.x;
            nAveragePoint.y += pPos.y - nSight.y;
        }
    }
    if(i > 0)
    {
        nAveragePoint.x = nAveragePoint.x / (i);
        nAveragePoint.y = nAveragePoint.y / (i);
    }

    int angle = getRadAngle(nAveragePoint.y, nAveragePoint.x);

    return angle;
}

SDL_bool perso_isAlive(perso p)
{
    if(p.status != dead)
    {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

SDL_bool perso_finishedAction(perso*p)
{
    SDL_bool finished = SDL_FALSE;

    if((    sprite_animationEnded(&p->sprites[attack]) && p->status == attack)
       || ( sprite_animationEnded(&p->sprites[hit]) && p->status == hit)
       || ( sprite_animationEnded(&p->sprites[farm]) && p->status == farm))
    {
        finished = SDL_TRUE;
    }
    return finished;
}

SDL_bool CircleInRect(SDL_Rect * circle, SDL_Rect * rect)
{
   const int32_t diameter = circle->w;

   int32_t x = (diameter/2 - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   vect2D centre;
   centre.x = circle->x + circle->w/2;
   centre.y = circle->y + circle->h/2;
   SDL_bool inRect = SDL_FALSE;

   while (x >= y && inRect == SDL_FALSE)
   {
      //  Each of the following renders an octant of the circle
      if(drawThePoint(centre.x + x, centre.y - y, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x + x, centre.y + y, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x - x, centre.y - y, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x - x, centre.y + y, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x + y, centre.y - x, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x + y, centre.y + x, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x - y, centre.y - x, rect))
        inRect = SDL_TRUE;
      if(drawThePoint(centre.x - y, centre.y + x, rect))
        inRect = SDL_TRUE;

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }

   return inRect;
}

SDL_bool drawThePoint(int x, int y, SDL_Rect * rect)
{
    SDL_bool inrect;
    SDL_Point point;
    point.x = x;
    point.y = y;
    inrect = SDL_PointInRect(&point, rect);
    return inrect;
}

SDL_bool isInFront(perso * p, perso * pFront)
{
    SDL_bool inFront = SDL_FALSE;

    for(int i = 0; i < p->nbNeighbours; i++)
    {
        float angle = perso_getRadAngle(*p, *pFront);
        if((p->gameObject.direction == east && (angle < 45 && angle > -45))
        || (p->gameObject.direction == west && (angle > 135 || angle <  -135))
        || (p->gameObject.direction == north && (angle > -135 && angle < -45))
        || (p->gameObject.direction == south && (angle > 45 && angle < 135)))
            if(p->neighbours[i] == pFront)
            {
                p->inFront = SDL_TRUE;
            }
    }
    return inFront;
}

void perso_blockHit(perso * p, int dmg)
{
    gameObject_addStamina(&p->gameObject, -dmg*3);
    if(p->gameObject.stamina <= 0)
        perso_setStatus(p, hit, NULL);
}

void perso_changeStance(perso * p, stance stance)
{
    if(p->status != block)
        p->gameObject.stance = stance;
    else
    {
        p->gameObject.stance = walking;
    }
}

SDL_bool perso_canMove(perso * p)
{
    SDL_bool movable = SDL_TRUE;
    if((p->status == hit && sprite_getFrame(p->sprites[hit]) <= 3) || p->status == attack || p->status == farm)
    {
        movable = SDL_FALSE;
    }
    return movable;
}

SDL_bool perso_isGettingAttacked(perso * p)
{
    SDL_bool attacked = SDL_FALSE;
    for(int i = 0; i < p->nbNeighbours; i++)
    {
        if(perso_getAttitude(p, p->neighbours[i]) <= 0 && p->neighbours[i]->status == attack)
        {
            attacked = SDL_TRUE;
            return attacked;
        }
    }

    return attacked;
}

SDL_bool perso_EnemyAroundHit(perso* p)
{

    SDL_bool hitd = SDL_FALSE;
    for(int i = 0; i < p->nbNeighbours; i++)
    {
        if(perso_getAttitude(p, p->neighbours[i]) <= 0 && p->neighbours[i]->status == hit)
        {
            hitd = SDL_TRUE;
            return hitd;
        }
    }

    return hitd;
}

SDL_bool perso_isMaxLife(perso p)
{
    return (p.gameObject.life >= p.gameObject.maxLife);
}

SDL_bool perso_isMaxStamina(perso p)
{
    return (p.gameObject.stamina >= p.gameObject.maxStamina);
}

void perso_lookAt(perso* p, perso * t)
{
    float angle = perso_getRadAngle(*p, *t);
    if(angle < 45 && angle > -45)
        p->gameObject.direction = east;
    else if(angle > 135 || angle <  -135)
        p->gameObject.direction = west;
    else if(angle > -135 && angle < -45)
        p->gameObject.direction = north;
    else if(angle > 45 && angle < 135)
        p->gameObject.direction = south;
}

SDL_bool perso_friendInFront(perso * p)
{
    SDL_bool inFront = SDL_FALSE;
    if(p->lastInFront != NULL && (p->lastInFront->gameObject.life <= 0 || (p->lastInFront->pTarget != p->pTarget && p->lastInFront != p->pTarget)))
        p->lastInFront = NULL;

    perso * lastFront;
    int distanceMax = p->neighboursDistance;
    if(p->status == idle)
        distanceMax = distanceMax *3/2;
    for(int i = 0; i < p->nbNeighbours; i++)
    {
        float angle = perso_getRadAngle(*p, *p->neighbours[i]);
        if((p->gameObject.direction == east && (angle < 45 && angle > -45))
        || (p->gameObject.direction == west && (angle > 135 || angle <  -135))
        || (p->gameObject.direction == north && (angle > -135 && angle < -45))
        || (p->gameObject.direction == south && (angle > 45 && angle < 135)))
        {
            lastFront = p->neighbours[i];
            if(p->lastInFront == NULL || lastFront == p->pTarget || getDistanceBetween(p->gameObject, p->lastInFront->gameObject) > distanceMax || !perso_TargetInFront(p))
                p->lastInFront = lastFront;
        }
    }


    if(perso_TargetInFront(p) && getDistanceBetween(p->gameObject, p->lastInFront->gameObject) < distanceMax)
    {
        inFront = SDL_TRUE;
        return inFront;
    }
    return inFront;
}

SDL_bool perso_TargetInFront(perso * lePerso)
{
    SDL_bool inFront = SDL_FALSE;
    int count = 0;
    perso ** pFront = (perso**)malloc(sizeof(perso*));
    if(lePerso->lastInFront != NULL)
    {
        pFront[count] = lePerso->lastInFront;
        while(pFront[count] != NULL)
        {
            if(pFront[count] == lePerso->pTarget)
            {
                inFront = SDL_TRUE;
                return inFront;
            }

            for(int i = 0; i < count; i++)
                if(pFront[i] == pFront[count])
                {
                    return inFront;
                }
            pFront = (perso**)realloc(pFront, (count+2)*(sizeof(perso*)));
            count++;
            pFront[count] = pFront[count-1]->lastInFront;
        }

    }
    return inFront;
}

float perso_getRadAngle(perso p1, perso p2)
{
    //RETURN ANGLE FROM -180 to 180 WHERE 0 IS EAST (CLOCKWISE)
    float direc1 = p1.gameObject.boxCollider.h;
    float direc2 = p2.gameObject.boxCollider.h;
    float direc3 = p1.gameObject.boxCollider.w;
    float direc4 = p2.gameObject.boxCollider.w;

    return getRadAngle((p2.gameObject.coord.y + direc1 / 2.0)
                     - (p1.gameObject.coord.y + direc2 / 2.0),
                       (p2.gameObject.coord.x + direc3 / 2.0)
                     - (p1.gameObject.coord.x + direc4 / 2.0));
}

fvect2D perso_moveToTarget(perso * lePerso, object * objects, int nbObject)
{
    gameObject_getBoxCollider(&lePerso->pTarget->gameObject);
    vect2D nTarget = gameObject_getPos(lePerso->pTarget->gameObject);

    return perso_moveToPos(lePerso, nTarget, objects, nbObject);
}

fvect2D perso_moveToPos(perso * lePerso, vect2D pos, object * objects, int nbObject)
{
    gameObject_getBoxCollider(&lePerso->gameObject);
    vect2D nObject = gameObject_getPos(lePerso->gameObject);
    if(lePerso->path->premier != NULL )
    {
        //READING THE PATH
        if((lePerso->path->premier->nombre.x == nObject.x &&
            lePerso->path->premier->nombre.y == nObject.y))
        {
            defiler(lePerso->path);
        }
    }
    fvect2D fCible;
    if(lePerso->path != NULL && lePerso->path->premier != NULL)
    {
        if(lePerso->path != NULL && lePerso->path->premier != NULL &&lePerso->path->premier->suivant != NULL)
        {
            while(lePerso->path->premier != NULL && lePerso->path->premier->suivant != NULL && line_isFree(*lePerso, objects, nbObject))
            {
                defiler(lePerso->path);
            }
        }
        if(lePerso->path->premier != NULL)
        {
            vect2D currentNTarget = getLastNumber(lePerso->path);
            if((currentNTarget.x != pos.x || currentNTarget.y != pos.y) && getNDistanceBetween(pos, currentNTarget) > getNDistanceBetween(currentNTarget, nObject) / 2)
                map_getPath(objects, nbObject, pos.x, pos.y, nObject.x, nObject.y, lePerso->path);
        }
    }
    else
    {
        map_getPath(objects, nbObject, pos.x, pos.y, nObject.x, nObject.y, lePerso->path);
    }
    fCible.x = lePerso->path->premier->nombre.x*TILESIZE;
    fCible.y = lePerso->path->premier->nombre.y*TILESIZE;

    return fCible;
}


void perso_emptyNeighbour(perso * p)
{
    p->neighbours = (perso**)realloc(p->neighbours, (sizeof(perso*)));
    p->nbNeighbours = 0;
}

void perso_addNeighbour(perso * p, perso * neighbour)
{
    p->neighbours = (perso**)realloc(p->neighbours, (p->nbNeighbours+1)*(sizeof(perso*)));
    p->neighbours[p->nbNeighbours] = neighbour;
    p->nbNeighbours++;
}

void perso_emptyInSight(perso * p)
{
    p->inSightCharacters = (perso**)realloc(p->inSightCharacters, (sizeof(perso*)));
    p->nbInSightCharacters = 0;
}

void perso_addInSightCharacters(perso * p, perso * neighbour)
{
    p->inSightCharacters = (perso**)realloc(p->inSightCharacters, (p->nbInSightCharacters+1)*(sizeof(perso*)));
    p->inSightCharacters[p->nbInSightCharacters] = neighbour;
    p->nbInSightCharacters++;
}

void perso_getFriends(perso * p, perso * persos, int nbPerso)
{
    perso_emptyNeighbour(p);
    for(int i = 0; i < nbPerso; i++)
    {
        if(p->gameObject.id != persos[i].gameObject.id &&
           getDistanceBetween(p->gameObject, persos[i].gameObject) < p->neighboursDistance)
        {
            perso_addNeighbour(p, &persos[i]);
        }
    }
    perso_emptyInSight(p);
    for(int i = 0; i < nbPerso; i++)
    {
        if(p->gameObject.id != persos[i].gameObject.id &&
           getDistanceBetween(p->gameObject, persos[i].gameObject) < p->sightRange)
        {
            perso_addInSightCharacters(p, &persos[i]);
        }
    }
}


vect2D neighbour_getAverageDir(perso * neighbours, int nbNeighbours)
{
    vect2D dir;
    dir.x = 0;
    dir.y = 0;
    for(int i = 0; i < nbNeighbours; i++)
    {
        dir.x += neighbours[i].gameObject.speed.x / nbNeighbours;
        dir.y += neighbours[i].gameObject.speed.y / nbNeighbours;
    }
    return dir;
}

fvect2D neighbour_getAvoidDir(perso * p, perso ** neighbours, int nbNeighbours)
{
    fvect2D steer;
    fvect2D pos = p->gameObject.coord;
    steer.x = 0;
    steer.y = 0;
    int count = 0;

    for (int i = 0; i < nbNeighbours; i++)
    {
        if(neighbours[i]->gameObject.passable == SDL_FALSE && perso_getAttitude(p, neighbours[i]) > 0 && getDistanceBetween(p->gameObject, neighbours[i]->gameObject) < p->avoidDistance)
        {
            fvect2D other = neighbours[i]->gameObject.coord;
            // Calculate vector pointing away from neighbor
            fvect2D diff;
            diff.x = pos.x - other.x;
            diff.y = pos.y - other.y;
            diff = normalizefVect2D(diff);     // Weight by distance
            diff = multifVect(diff, (1.1-fNormalize(getDistfVect2D(pos, other), p->gameObject.boxCollider.w-1, p->avoidDistance)) * p->gameObject.maxS);
            steer = addfVect2D(diff, steer);
            count++;
        }
    }
    if(count > 0)
    {
        steer.x /=count;
        steer.y /=count;
        if(steer.x < 0.1)
            steer.x = p->gameObject.maxS /5 *(rand()%3 - 1);
        if(steer.y < 0.1)
            steer.y = p->gameObject.maxS /5 *(rand()%3 - 1);

    }
    //steer.x *=1.5;
    //steer.y *=1.5;
    return steer;
}

void perso_kill(perso * p)
{
    p->gameObject.life = 0;
}

void perso_heal(perso * p)
{
    p->gameObject.life = p->gameObject.maxLife;
    p->status = idle;
}

int perso_setLiege(perso * p, perso * l)
{
    if(p != l && !perso_isLiege(p, p))
    {
        p->liege = l;
        return 1;

    }
    printf("Couldn't change vassal of %s\n", p->gameObject.name);
    return 0;
}

void perso_moveUp(perso * p)
{
    fvect2D v;
    v.x = 0;
    v.y = -p->gameObject.maxS;
    addGoalV(&p->gameObject, v);

}

void perso_moveDown(perso * p)
{
    fvect2D v;
    v.x = 0;
    v.y = p->gameObject.maxS;
    addGoalV(&p->gameObject, v);
}

void perso_moveRight(perso * p)
{
    fvect2D v;
    v.x = p->gameObject.maxS;
    v.y = 0;
    addGoalV(&p->gameObject, v);
}

void perso_moveLeft(perso * p)
{
    fvect2D v;
    v.x = -p->gameObject.maxS;
    v.y = 0;
    addGoalV(&p->gameObject, v);
}

void perso_slowX(perso * p)
{
    setGoalVX(&p->gameObject, 0);
}

void perso_slowY(perso * p)
{
    setGoalVY(&p->gameObject, 0);
}

int perso_getHitRange(perso * p)
{
    return p->hitRange;
}


sprite perso_getSprite(perso * p, status status)
{
    return p->sprites[status];
}


int collision(perso p1, perso p2)
{
    int colli = 0;

    if(SDL_HasIntersection(&p1.gameObject.boxCollider, &p2.gameObject.boxCollider))
    {
        colli = 1;
    }

    return colli;
}

void perso_destroy(perso * p)
{
    for(int i = 0; i < dead + 1; i++)
    {
        if(p->sprites[i].texture != NULL)
            SDL_DestroyTexture(p->sprites[i].texture);
    }
    free(p->weapon);
    free(p->pTarget);
}

/*
void perso_setTarget(perso * p, perso * target)
{
    &p->pTarget->gameObject = target;
}*/


