#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

#include "sprite.h"
#include "gameObject.h"
#include "boxDrawer.h"
#include "weapon.h"
#include "IA.h"
#include <math.h>

#define CHARACPATH "characters/"

typedef enum race{human, undead}race; //KEEP UNDEAD THE LAST ONE
typedef enum statue{idle, walk, attack, block, hit, dead}statue; //KEEP DEAD LAST

//Contiens les différent types de perso
typedef struct perso perso;
struct perso{
    char name[255];
    statue statue;
    int range;
    int hitRange;
    race race;
    int raceAttitude[undead+1];
    int attackFrame;
    SDL_bool playerControled;
    vect2D attackBoxOffset;

    sprite sprites[dead+1];

    gameObject gameObject;
    gameObject * pTarget;
    File * path;
    perso ** neighbours;
    int nbNeighbours;
    int neighboursDistance;

    weapon * weapon;
};
/*
typedef struct pTarget {
    perso * p;
}pTarget;*/

void perso_INI(perso * p, SDL_Renderer * renderer, perso * model, int x, int y);
void newModel_INI(perso * lePerso, char * name, int maxLife, float maxV, float acc, SDL_bool movable, SDL_bool passable, int race, int attackFrame, int range, int hitRange, sprite sprites[dead + 1], weapon * w);
void perso_setWalk(struct perso * lePerso);
void perso_setAttack(struct perso * lePerso);
void perso_setHitted(perso * p, perso * p2, weapon * weapon);
void perso_setDead(perso * p);
void perso_setIdle(perso * p);
void perso_setTarget(perso * lePerso, gameObject * target);

void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY, SDL_bool debugOn);
void perso_MOVETO(struct perso * lePerso, int posX, int posY);
void perso_Simulate(perso * lePerso, char sMap[255], Uint32 ticks);
gameObject * perso_getGameObject(perso * p);
int perso_getHitRange(perso * p);
int collision(perso p1, perso p2);
sprite perso_getSprite(perso * p, statue statue);
int perso_getRaceAttitude(perso * p, race r);
int getDetecRange(perso p);
sprite * model_getSprites(perso * model);
void perso_moveUp(perso * p);
void perso_moveDown(perso * p);
void perso_moveRight(perso * p);
void perso_moveLeft(perso * p);
void perso_slowX(perso * p);
void perso_slowY(perso * p);
void perso_getFriends(perso * p, perso * persos, int nbPerso);
void perso_addNeighbour(perso * p, perso * neighbour);
void perso_emptyNeighbour(perso * p);
void perso_getFriends(perso * p, perso * persos, int nbPerso);

void perso_INI(perso * p, SDL_Renderer * renderer, perso * model, int x, int y)
{
    if(model == NULL || p == NULL)
    {
        printf("INI Character : echec\n");
        exit(0);
    }
    //COPY FROM MODEL
    p->weapon = (weapon*)malloc(sizeof(weapon*));
    p->pTarget = (gameObject*)malloc(sizeof(gameObject*));
	p->neighbours = (perso**)malloc(sizeof(perso*));
    *p = *model;
    p->pTarget = NULL;
	p->neighbours = NULL;
	p->nbNeighbours = 0;
	p->neighboursDistance = 100;
    gameObject_INI(&p->gameObject, model->gameObject.maxLife, x, y, model->gameObject.maxS, model->gameObject.a, model->gameObject.movable, model->gameObject.passable);
    p->path = initialiser();/*
    for(int i = 0; i < dead + 1; i++)
        p->sprites[i] = model->sprites[i];

    p->race = model->race;
    for(int i = 0; i < undead + 1; i++)
        p->raceAttitude[i] = model->raceAttitude[i];

    p->attackFrame = model->attackFrame;
    p->statue = idle;
    p->following = 0;
    p->gameObject.life = p->gameObject.maxLife;
    p->hitRange = model->hitRange;
    strcpy(p->name, model->name);
    p->path = initialiser();
    p->playerControled = 0;
    p->range = model->range;
    p->weapon = model->weapon;
    p->pTarget = NULL;
    p->attackBoxOffset = model->attackBoxOffset;
    */

}

void newModel_INI(perso * lePerso, char * name, int maxLife, float maxV, float acc, SDL_bool movable, SDL_bool passable, int race, int attackFrame, int range, int hitRange, sprite sprites[dead + 1], weapon * w)
{
    gameObject_INI(&lePerso->gameObject, maxLife, 100, 100, maxV, acc, movable, passable);
    strcpy(lePerso->name, name);
    lePerso->race = race;
    lePerso->attackFrame = attackFrame;
    lePerso->weapon = (weapon*)malloc(sizeof(weapon*));
    lePerso->weapon = w;
    lePerso->pTarget = (gameObject*)malloc(sizeof(gameObject*));
    lePerso->pTarget = NULL;
    lePerso->neighbours = (perso**)malloc(sizeof(perso*));
    lePerso->neighbours = NULL;

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

    lePerso->attackBoxOffset.x = sprite_getWidth(perso_getSprite(lePerso, walk));
    lePerso->attackBoxOffset.y = sprite_getHeight(perso_getSprite(lePerso, walk))/2;
    lePerso->attackFrame = attackFrame;
    lePerso->range = range;
    lePerso->hitRange = hitRange;

    lePerso->gameObject.boxColliderOffset.x = 0;
    lePerso->gameObject.boxColliderOffset.y = sprite_getHeight(lePerso->sprites[walk]) - lePerso->gameObject.boxCollider.h;
    gameObject_getBoxCollider(&lePerso->gameObject);

    lePerso->statue = idle;
    lePerso->playerControled = SDL_FALSE;
    lePerso->path = initialiser();
}

sprite * model_getSprites(perso * model)
{
    return model->sprites;
}

void perso_equipWeapon(perso * p, weapon * w)
{
    p->weapon = w;
}


int perso_getRaceAttitude(perso * p, race r)
{
    return p->raceAttitude[r];
}

int getDetecRange(perso p)
{
    return p.range;
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

void perso_setStatue(struct perso * p, statue statue, perso * p2)
{
	if(statue == hit)
	{
		if((p->statue != hit && p->statue != dead) || ((p->statue == hit && sprite_getFrame(p->sprites[hit]) > 1) && p2 != NULL))
		{
			p->statue = hit;
			sprite_RESET(&p->sprites[hit]);
			gameObject_addLife(&p->gameObject, -p2->weapon->damage);
			//printf("ID(%i) suffer %i damage(%i/%i) from %s\n", p->gameObject.id, p2->weapon->damage, p->gameObject.life, p->gameObject.maxLife, p2->weapon->name);
			if(p2 != NULL && p->pTarget != &p2->gameObject)
			{
				perso_setTarget(p, perso_getGameObject(p2));
				//printf("ID(%i) targetting ID(%i)\n\n", p->gameObject.id, p2->gameObject.id);
			}
		}
	}
    else if(p->statue != statue && p->statue != dead)
    {
        p->statue = statue;
        sprite_RESET(&p->sprites[statue]);
    }
}


gameObject * perso_getGameObject(perso * p)
{
    return &p->gameObject;
}


void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY, SDL_bool debugOn)
{
    if(lePerso->gameObject.direction == east)
    {
        for(int i = 0; i < dead; i++)
            sprite_FLIP_RIGHT(&lePerso->sprites[i]);
        if(sprite_DRAW(&lePerso->sprites[lePerso->statue], lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY))
            lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
    }

    else if(lePerso->gameObject.direction == west)
    {
        for(int i = 0; i < dead; i++)
            sprite_FLIP_LEFT(&lePerso->sprites[i]);
        if(sprite_DRAW(&lePerso->sprites[lePerso->statue], lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY))
            lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
    }
    else if(lePerso->gameObject.direction == north)
    {
        if(sprite_DRAW(&lePerso->sprites[lePerso->statue], lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY))
            lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
    }
    else if(lePerso->gameObject.direction == south)
    {
        if(sprite_DRAW(&lePerso->sprites[lePerso->statue], lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.coord.x + offsetX, lePerso->gameObject.coord.y + offsetY))
            lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
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

void perso_setTarget(perso * lePerso, gameObject * target)
{
    if(lePerso->playerControled == 0)
        lePerso->pTarget = target;
}

void perso_Simulate(perso * lePerso, char sMap[255], Uint32 ticks)
{
    gameObject_getBoxCollider(&lePerso->gameObject);
    if(gameObject_getLife(perso_getGameObject(lePerso)) <= 0)
    {
        perso_setStatue(lePerso, dead, NULL);
    }
    if(lePerso->statue != dead)
    {
        if(gameObject_getLife(lePerso->pTarget) <= 0)
        {
            lePerso->pTarget = NULL;
        }

        //Simuler le personnage
        if(lePerso->playerControled == SDL_FALSE)
            {
                if(lePerso->statue == idle || lePerso->statue == walk)
                {
                    //Effectuer les déplacements
                    gameObject_simulate(&lePerso->gameObject, lePerso->pTarget, sMap, lePerso->path);
                    if(lePerso->pTarget != NULL)
                    {
                        if(lePerso->weapon->weaponType == range)
                        {
                            if(getDistanceBetween(&lePerso->gameObject, lePerso->pTarget) < perso_getHitRange(lePerso))
                                perso_setStatue(lePerso, attack, NULL);
                        }
                        else
                        {
                            if(SDL_HasIntersection(perso_getAttackBox(lePerso), gameObject_getBoxCollider(lePerso->pTarget)))
                            {
                                perso_setStatue(lePerso, attack, NULL);
                            }
                        }
                    }

                }
                else
                {
                    gameObject_slow(&lePerso->gameObject.speed.x, lePerso->gameObject.a);
                    gameObject_slow(&lePerso->gameObject.speed.y, lePerso->gameObject.a);
                }
            }
            else
            {
                updateV(&lePerso->gameObject);
            }
                if(lePerso->gameObject.speed.x == 0 && lePerso->gameObject.speed.y == 0 )
                {
                    if(lePerso->statue == walk)
                    {
                        perso_setStatue(lePerso, idle, NULL);
                    }
                }
                else
                {
                    if(lePerso->statue == idle)
                    {
                        perso_setStatue(lePerso, walk, NULL);
                    }
                }

            gameObject_move(&lePerso->gameObject);

            if(sprite_animationEnded(lePerso->sprites[attack]) && lePerso->statue == attack)
            {
                perso_setStatue(lePerso, walk, NULL);
                sprite_RESET(&lePerso->sprites[attack]);
            }
            else if(sprite_animationEnded(lePerso->sprites[hit]) && lePerso->statue == hit)
            {
                perso_setStatue(lePerso, walk, NULL);
            }

            if(lePerso->pTarget == NULL && lePerso->statue == idle && lePerso->playerControled == 0)
            {
                gameObject_slow(&lePerso->gameObject.speed.x, lePerso->gameObject.a);
                gameObject_slow(&lePerso->gameObject.speed.y, lePerso->gameObject.a);
            }

            //lePerso->gameObject.timeSinceLastMove = ticks;
    }
}

void perso_getFriends(perso * p, perso * persos, int nbPerso)
{
    perso_emptyNeighbour(p);
    for(int i = 0; i < nbPerso; i++)
    {
        if(p->gameObject.id != persos[i].gameObject.id && getDistanceBetween(&p->gameObject, &persos[i].gameObject) < p->neighboursDistance)
        {
            perso_addNeighbour(p, &persos[i]);
        }
    }
}

void perso_emptyNeighbour(perso * p)
{
    p->neighbours = (perso**)realloc(p->neighbours, (1)*(sizeof(perso*)));
    p->nbNeighbours = 0;
}

void perso_addNeighbour(perso * p, perso * neighbour)
{
    p->neighbours = (perso**)realloc(p->neighbours, (p->nbNeighbours+1)*(sizeof(perso*)));
    p->neighbours[p->nbNeighbours] = neighbour;
    p->nbNeighbours++;
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

vect2D neighbour_getAvoidDir(perso * neighbours, int nbNeighbours)
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


void perso_moveUp(perso * p)
{
    perso_setStatue(p, walk, NULL);
    p->gameObject.direction = north;
    fvect2D v;
    v.x = 0;
    v.y = -p->gameObject.maxS;
    addGoalV(&p->gameObject, v);

}

void perso_moveDown(perso * p)
{
    perso_setStatue(p, walk, NULL);
    p->gameObject.direction = south;
    fvect2D v;
    v.x = 0;
    v.y = p->gameObject.maxS;
    addGoalV(&p->gameObject, v);
}

void perso_moveRight(perso * p)
{
    perso_setStatue(p, walk, NULL);
    p->gameObject.direction = east;
    fvect2D v;
    v.x = p->gameObject.maxS;
    v.y = 0;
    addGoalV(&p->gameObject, v);
}

void perso_moveLeft(perso * p)
{
    perso_setStatue(p, walk, NULL);
    p->gameObject.direction = west;
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


sprite perso_getSprite(perso * p, statue statue)
{
    return p->sprites[statue];
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
    p->pTarget = target;
}*/



#endif // PERSO_H_INCLUDED
