#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

#include "sprite.h"
#include "gameObject.h"
#include "boxDrawer.h"
#include "weapon.h"
#include "IA.h"
#include <math.h>

typedef enum race{undead, human}race;

//Structure perso qui contient gameObject
//Contiens les différent types de perso
typedef struct perso {
    int attacking;
    int following;
    int damage;
    int range;
    int hitRange;
    race race;
    int raceAttitude[human+1];
    int attackFrame;
    SDL_bool playerControled;
    vect2D attackBoxOffset;

    sprite sprIdle;
    sprite sprBlock;
    sprite sprDead;
    sprite sprWalk;
    sprite sprAttack;
    sprite sprHit;

    gameObject gameObject;
    gameObject * pTarget;
    File * path;

    weapon * weapon;
    //pTarget target;
}perso;
/*
typedef struct pTarget {
    perso * p;
}pTarget;*/

void perso_INI(struct perso * lePerso, SDL_Renderer * renderer, int t, int x, int y);
void perso_setWalk(struct perso * lePerso);
void perso_setAttack(struct perso * lePerso);
void perso_setHitted(perso * p, perso * p2, weapon * weapon);
void perso_setDead(perso * p);
void perso_setIdle(perso * p);
void perso_setTarget(perso * lePerso, gameObject * target);

void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY, SDL_bool debugOn);
void perso_MOVETO(struct perso * lePerso, int posX, int posY);
void perso_Simulate(perso * lePerso, char sMap[50], Uint32 ticks);
gameObject * perso_getGameObject(perso * p);
int perso_getHitRange(perso * p);
int collision(perso p1, perso p2);
SDL_Rect * perso_getHitBox(perso * p);
sprite perso_getAttackSprite(perso * p);
sprite perso_getWalkSprite(perso * p);
sprite perso_getIdleSprite(perso * p);
int perso_getRaceAttitude(perso * p, race r);
int getDetecRange(perso p);
int getDistanceBetween(perso * p, gameObject * o);

void perso_moveUp(perso * p);
void perso_moveDown(perso * p);
void perso_moveRight(perso * p);
void perso_moveLeft(perso * p);
void perso_slowX(perso * p);
void perso_slowY(perso * p);
int getDistanceBetween(perso * p, gameObject * o);

void perso_INI(struct perso * lePerso, SDL_Renderer * renderer, int t, int x, int y)
{

    char tPersoA[50];
    char tPersoB[50];
    char tPersoW[50];
    char tPersoH[50];
    char tPersoD[50];
    char tPersoI[50];
        lePerso->gameObject.timeSinceLastFrame = 0;

    switch (t)
    {
        case 0 :
            gameObject_INI(&lePerso->gameObject, 30, x, y, 4, 0.8, 1, renderer);
            strcpy(tPersoA, "Skeleton_Attack.png");
            strcpy(tPersoW, "Skeleton_Walk.png");
            strcpy(tPersoH, "Skeleton_Hit.png");
            strcpy(tPersoD, "Skeleton_Dead.png");
            strcpy(tPersoI, "Skeleton_Idle.png");
            sprite_INI(&lePerso->sprWalk, tPersoW, 0, 0, 22, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprWalk, 70);

            sprite_INI(&lePerso->sprAttack, tPersoA, 8, -3, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprAttack, 80);

            sprite_INI(&lePerso->sprHit, tPersoH, 0, 1, 30, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprHit, 80);

            sprite_INI(&lePerso->sprIdle, tPersoI, 0, 0, 24, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprIdle, 110);

            sprite_INI(&lePerso->sprDead, tPersoD, -1, 0, 33, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprDead, 90);

            lePerso->race = undead;
            lePerso->raceAttitude[undead] = 10;
            lePerso->raceAttitude[human] = -10;
            lePerso->attackBoxOffset.x = sprite_getWidth(perso_getWalkSprite(lePerso));
            lePerso->attackBoxOffset.y = sprite_getHeight(perso_getWalkSprite(lePerso))/2;
            lePerso->attackFrame = 7;
            lePerso->range = 450;
            lePerso->damage = 15;
            lePerso->hitRange = 130;


            break;

        case 1 :
            gameObject_INI(&lePerso->gameObject, 60, x, y, 6, 1, 1, renderer);
            strcpy(tPersoA, "knight_attack.png");
            strcpy(tPersoB, "knight_block.png");
            strcpy(tPersoW, "knight_walk2.png");
            strcpy(tPersoH, "knight_hit.png");
            strcpy(tPersoD, "knight_death.png");
            strcpy(tPersoI, "knight_idle.png");

            sprite_INI(&lePerso->sprWalk, tPersoW, 0, 0, 42, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprWalk, 90);

            sprite_INI(&lePerso->sprAttack, tPersoA, 18, -20, 80, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprAttack, 70);

            sprite_INI(&lePerso->sprBlock, tPersoD, 0, 0, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprBlock, 80);

            sprite_INI(&lePerso->sprHit, tPersoH, 0, 0, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprHit, 100);

            sprite_INI(&lePerso->sprIdle, tPersoI, 0, 0, 42, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprIdle, 150);

            sprite_INI(&lePerso->sprDead, tPersoD, 0, 0, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprDead, 150);

            lePerso->race = human;
            lePerso->raceAttitude[undead] = -30;
            lePerso->raceAttitude[human] = 10;
            lePerso->hitRange = 90;
            lePerso->attackBoxOffset.x = 20;
            lePerso->attackBoxOffset.y = sprite_getHeight(perso_getWalkSprite(lePerso))/2;
            lePerso->attackFrame = 5;
            lePerso->range = 600;
            lePerso->damage = 10;

            break;

    }

    lePerso->gameObject.hitBox.x = lePerso->gameObject.X;
    lePerso->gameObject.hitBox.y = lePerso->gameObject.Y;
    lePerso->gameObject.hitBox.w = sprite_getWidth(perso_getIdleSprite(lePerso))/2;
    lePerso->gameObject.hitBox.h = sprite_getHeight(perso_getIdleSprite(lePerso))/2;


    lePerso->gameObject.boxCollider.w = TILESIZE;
    lePerso->gameObject.boxCollider.h = TILESIZE;
    lePerso->gameObject.boxColliderOffset.x = 0;
    lePerso->gameObject.boxColliderOffset.y = sprite_getHeight(lePerso->sprWalk) - lePerso->gameObject.boxCollider.h;
    //lePerso->gameObject.boxCollider.h = lePerso->sprWalk.imgPos.h/2;
    lePerso->attacking = 0;
    lePerso->playerControled = SDL_FALSE;
    lePerso->pTarget = NULL;
    lePerso->path = initialiser();
}
void perso_equipWeapon(perso * p, weapon * w)
{
    p->weapon = w;
}

int getDistanceBetween(perso * p, gameObject * o)
{
    int dist = 10000;
    if(o != NULL)
    {
        dist = abs(p->gameObject.X - o->X) + abs(p->gameObject.Y - o->Y);
    }
    else
    {
        dist = p->range;
    }
    return dist;
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
    if(p->sprAttack.flip == SDL_FLIP_NONE)
    {
        p->weapon->hitBox.x = p->gameObject.X + p->gameObject.hitBox.w;
        p->weapon->hitBox.y = p->gameObject.Y + p->gameObject.hitBox.h + p->weapon->hitBox.h/2;
    }
    else
    {
        p->weapon->hitBox.x = p->gameObject.X - p->weapon->hitBox.w;
        p->weapon->hitBox.y = p->gameObject.Y + p->gameObject.hitBox.h + p->weapon->hitBox.h/2;
    }
    return &p->weapon->hitBox;
}

void perso_setWalk(struct perso * p)
{
    /**\brief
     *
     * \param lePerso->attacking
     *
     */
    if(p->attacking != 0)
    {
        p->attacking = 0;
        sprite_RESET(&p->sprWalk);
    }
}

void perso_setAttack(struct perso * p)
{
    if(p->attacking != 1 && p->gameObject.life > 0 && p->attacking != 2)
    {
        p->attacking = 1;
        sprite_RESET(&p->sprAttack);
    }
}

void perso_setHitted(perso * p, perso * p2, weapon * weapon)
{
    // if(p->attacking != 2 && p->attacking != 4)
    if((p->attacking != 2 && p->attacking != 4) || (p->attacking == 2 && sprite_getFrame(p->sprHit) > 1))
    {
        p->attacking = 2;
        sprite_RESET(&p->sprHit);
        gameObject_addLife(&p->gameObject, -weapon->damage);
        printf("ID(%i) suffer %i damage(%i/%i) from %s\n", p->gameObject.id, weapon->damage, p->gameObject.life, p->gameObject.maxLife, weapon->name);
        if(p2 != NULL && p->pTarget != p2)
        {
            perso_setTarget(p, perso_getGameObject(p2));
            printf("ID(%i) targetting ID(%i)\n\n", p->gameObject.id, p2->gameObject.id);
        }
    }
}

void perso_setDead(perso * p)
{
    if(p->attacking != 4)
    {
        p->attacking = 4;
        sprite_RESET(&p->sprDead);
    }
}

void perso_setIdle(perso * p)
{
    if(p->attacking !=3)
    {
        p->attacking = 3;
        sprite_RESET(&p->sprIdle);
    }
}

gameObject * perso_getGameObject(perso * p)
{
    return &p->gameObject;
}


void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY, SDL_bool debugOn)
{
    switch (lePerso->attacking)
    {
        case 0 :
            if(sprite_DRAW(&lePerso->sprWalk, lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY))
                lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
        break;
        case 1 :
            if(sprite_DRAW(&lePerso->sprAttack, lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY))
                lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
        break;
        case 2 :
            if(sprite_DRAW(&lePerso->sprHit, lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY))
                lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
        break;
        case 3 :
            if(sprite_DRAW(&lePerso->sprIdle, lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY))
                lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
        break;
        case 4 :
            if(sprite_FDRAW(&lePerso->sprDead, lePerso->gameObject.timeSinceLastFrame, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY))
                lePerso->gameObject.timeSinceLastFrame = SDL_GetTicks();
        break;
    }
    if(debugOn)
    {
        SDL_Color cBlue;
        cBlue.a = 0;
        cBlue.b = 150;
        cBlue.r = 50;
        cBlue.g = 50;
        SDL_Color cRed;
        cRed.a = 255;
        cRed.b = 50;
        cRed.r = 150;
        cRed.g = 50;
        SDL_Color cGreen;
        cRed.a = 200;
        cRed.b = 50;
        cRed.r = 50;
        cRed.g = 150;

        SDL_Rect attackBox;
        attackBox.x = lePerso->weapon->hitBox.x + offsetX;
        attackBox.y = lePerso->weapon->hitBox.y + offsetY;
        attackBox.h = lePerso->weapon->hitBox.h;
        attackBox.w = lePerso->weapon->hitBox.w;
        SDL_Rect hitBox;
        hitBox.x = lePerso->gameObject.hitBox.x + offsetX;
        hitBox.y = lePerso->gameObject.hitBox.y + offsetY;
        hitBox.h = lePerso->gameObject.hitBox.h;
        hitBox.w = lePerso->gameObject.hitBox.w;
        SDL_Rect boxCollider;
        boxCollider.x = lePerso->gameObject.boxCollider.x + offsetX;
        boxCollider.y = lePerso->gameObject.boxCollider.y + offsetY;
        boxCollider.h = lePerso->gameObject.boxCollider.h;
        boxCollider.w = lePerso->gameObject.boxCollider.w;

        draw_Box(attackBox, cBlue, renderer);
        draw_Box(hitBox, cRed, renderer);
        draw_Box(boxCollider, cGreen, renderer);
    }

}

void perso_MOVETO(struct perso * lePerso, int posX, int posY)
{
    /*
    if(posX < lePerso->gameObject.X)
    {
        sprite_FLIP_LEFT(&lePerso->sprAttack);
        sprite_FLIP_LEFT(&lePerso->sprWalk);
        sprite_FLIP_LEFT(&lePerso->sprHit);
        sprite_FLIP_LEFT(&lePerso->sprDead);
        sprite_FLIP_LEFT(&lePerso->sprIdle);
    }

    if(posX > lePerso->gameObject.X)
    {
        sprite_FLIP_RIGHT(&lePerso->sprAttack);
        sprite_FLIP_RIGHT(&lePerso->sprWalk);
        sprite_FLIP_RIGHT(&lePerso->sprHit);
        sprite_FLIP_RIGHT(&lePerso->sprDead);
        sprite_FLIP_RIGHT(&lePerso->sprIdle);
    }*/
    lePerso->gameObject.X = posX;
    lePerso->gameObject.Y = posY;
}

void perso_setTarget(perso * lePerso, gameObject * target)
{
    if(lePerso->playerControled == 0)
        lePerso->pTarget = target;
}

void perso_Simulate(perso * lePerso, char sMap[50], Uint32 ticks)
{
    gameObject_getHitBox(&lePerso->gameObject);
    if(gameObject_getLife(perso_getGameObject(lePerso)) <= 0)
    {
        perso_setDead(lePerso);
    }
    if(lePerso->attacking != 4)
    {
        if(gameObject_getLife(lePerso->pTarget) <= 0)
        {
            lePerso->pTarget = NULL;
        }

        //Simuler le personnage
        if(lePerso->playerControled == SDL_FALSE)
            {

                if(sprite_getFrame(perso_getAttackSprite(lePerso)) < lePerso->attackFrame - 2)
                {
                    if(lePerso->path->premier != NULL)
                    {
                        if(lePerso->path->premier->suivant != NULL)
                        {
                            //printf("%i %i\n", lePerso->path->premier->suivant->nombre.x, (int)(lePerso->gameObject.X+TILESIZE/2)/TILESIZE);
                            if(lePerso->path->premier->suivant->nombre.x > (int)(lePerso->gameObject.X+TILESIZE/2)/TILESIZE)
                            {
                                sprite_FLIP_RIGHT(&lePerso->sprAttack);
                                sprite_FLIP_RIGHT(&lePerso->sprWalk);
                                sprite_FLIP_RIGHT(&lePerso->sprHit);
                                sprite_FLIP_RIGHT(&lePerso->sprDead);
                                sprite_FLIP_RIGHT(&lePerso->sprIdle);
                            }
                            else
                            {
                                sprite_FLIP_LEFT(&lePerso->sprAttack);
                                sprite_FLIP_LEFT(&lePerso->sprWalk);
                                sprite_FLIP_LEFT(&lePerso->sprHit);
                                sprite_FLIP_LEFT(&lePerso->sprIdle);
                                sprite_FLIP_LEFT(&lePerso->sprDead);
                            }
                        }
                    }
                }
                if(lePerso->attacking == 0 || lePerso->attacking == 3 )
                {
                    if(getDistanceBetween(lePerso, lePerso->pTarget) < perso_getHitRange(lePerso))
                    {
                        perso_setAttack(lePerso);
                    }
                    //Verifier la position de sa "target"

                    //Effectuer les déplacements
                    gameObject_simulate(&lePerso->gameObject, lePerso->pTarget, sMap, lePerso->path);

                }
                else
                {
                    gameObject_slow(&lePerso->gameObject.vX, lePerso->gameObject.a);
                    gameObject_slow(&lePerso->gameObject.vY, lePerso->gameObject.a);
                }
            }
                if(lePerso->gameObject.vX == 0 && lePerso->gameObject.vY == 0 )
                {
                    if(lePerso->attacking == 0)
                    {
                        perso_setIdle(lePerso);
                    }
                }
                else
                {
                    if(lePerso->attacking == 3)
                    {
                        perso_setWalk(lePerso);
                    }
                }

            gameObject_move(&lePerso->gameObject);

            if(sprite_animationEnded(lePerso->sprAttack) && lePerso->attacking == 1)
            {
                perso_setWalk(lePerso);
                sprite_RESET(&lePerso->sprAttack);
            }
            else if(sprite_animationEnded(lePerso->sprHit) && lePerso->attacking == 2)
            {
                perso_setWalk(lePerso);
            }

            if(lePerso->pTarget == NULL && lePerso->attacking == 0 && lePerso->playerControled == 0)
            {
                gameObject_slow(&lePerso->gameObject.vX, lePerso->gameObject.a);
                gameObject_slow(&lePerso->gameObject.vY, lePerso->gameObject.a);
            }

            //lePerso->gameObject.timeSinceLastMove = ticks;
    }
}

void perso_moveUp(perso * p)
{
    if(p->gameObject.vY - 3 * p->gameObject.a > p->gameObject.a)
        gameObject_slow(&p->gameObject.vY, p->gameObject.a);
    addVY(&p->gameObject,-p->gameObject.a);

}

void perso_moveDown(perso * p)
{
    if(p->gameObject.vY + 3 * p->gameObject.a < p->gameObject.a)
        gameObject_slow(&p->gameObject.vY, p->gameObject.a);
    addVY(&p->gameObject,p->gameObject.a);
}

void perso_moveRight(perso * p)
{
    if(p->sprAttack.flip != SDL_FLIP_NONE)
        sprite_FLIP_RIGHT(&p->sprAttack);
    if(p->sprWalk.flip != SDL_FLIP_NONE)
        sprite_FLIP_RIGHT(&p->sprWalk);
    if(p->sprHit.flip != SDL_FLIP_NONE)
        sprite_FLIP_RIGHT(&p->sprHit);
    if(p->sprDead.flip != SDL_FLIP_NONE)
        sprite_FLIP_RIGHT(&p->sprDead);
    if(p->sprIdle.flip != SDL_FLIP_NONE)
        sprite_FLIP_RIGHT(&p->sprIdle);
    addVX(&p->gameObject,p->gameObject.a);
}

void perso_moveLeft(perso * p)
{
    if(p->sprAttack.flip != SDL_FLIP_HORIZONTAL)
        sprite_FLIP_LEFT(&p->sprAttack);
    if(p->sprWalk.flip != SDL_FLIP_HORIZONTAL)
        sprite_FLIP_LEFT(&p->sprWalk);
    if(p->sprHit.flip != SDL_FLIP_HORIZONTAL)
        sprite_FLIP_LEFT(&p->sprHit);
    if(p->sprIdle.flip != SDL_FLIP_HORIZONTAL)
        sprite_FLIP_LEFT(&p->sprIdle);
    if(p->sprDead.flip != SDL_FLIP_HORIZONTAL)
        sprite_FLIP_LEFT(&p->sprDead);

    addVX(&p->gameObject,-p->gameObject.a);
}

void perso_slowX(perso * p)
{
    gameObject_slow(&p->gameObject.vX, p->gameObject.a);
}

void perso_slowY(perso * p)
{
    gameObject_slow(&p->gameObject.vY, p->gameObject.a);
}

int perso_getHitRange(perso * p)
{
    return p->hitRange;
}

sprite perso_getAttackSprite(perso * p)
{
    return p->sprAttack;
}

sprite perso_getWalkSprite(perso * p)
{
    return p->sprWalk;
}

sprite perso_getIdleSprite(perso * p)
{
    return p->sprIdle;
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

/*
void perso_setTarget(perso * p, perso * target)
{
    p->pTarget = target;
}*/



#endif // PERSO_H_INCLUDED
