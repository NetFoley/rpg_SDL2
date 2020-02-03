#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

#include "sprite.h"
#include "gameObject.h"
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

    SDL_Rect attackBox;
    //pTarget target;
}perso;
/*
typedef struct pTarget {
    perso * p;
}pTarget;*/

void perso_INI(struct perso * lePerso, SDL_Renderer * renderer, int t);
void perso_setWalk(struct perso * lePerso);
void perso_setAttack(struct perso * lePerso);
void perso_setHitted(perso * p, perso * p2, int dmg);
void perso_setDead(perso * p);
void perso_setIdle(perso * p);
void perso_setTarget(perso * lePerso, gameObject * target);

void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY);
void perso_MOVETO(struct perso * lePerso, int posX, int posY);
void perso_Simulate(perso * lePerso, Uint32 ticks);
gameObject * perso_getGameObject(perso * p);
int perso_getHitRange(perso * p);
int collision(perso p1, perso p2);
SDL_Rect * perso_getHitBox(perso * p);
sprite perso_getAttackSprite(perso * p);
sprite perso_getWalkSprite(perso * p);
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

void perso_INI(struct perso * lePerso, SDL_Renderer * renderer, int t)
{

    char tPersoA[50];
    char tPersoB[50];
    char tPersoW[50];
    char tPersoH[50];
    char tPersoD[50];
    char tPersoI[50];

    static int i = 0;

    i = i + TILESIZE*5;
    switch (t)
    {
        case 0 :
            strcpy(tPersoA, "Skeleton_Attack.png");
            strcpy(tPersoW, "Skeleton_Walk.png");
            strcpy(tPersoH, "Skeleton_Hit.png");
            strcpy(tPersoD, "Skeleton_Dead.png");
            strcpy(tPersoI, "Skeleton_Idle.png");
            sprite_INI(&lePerso->sprWalk, tPersoW, 0, 0, 22, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprWalk, 90);

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
            lePerso->attackBoxOffset.x = 60;
            lePerso->attackBoxOffset.y = sprite_getHeight(perso_getWalkSprite(lePerso))/2;
            gameObject_INI(&lePerso->gameObject, 30, i , 300, 4, 0.35, 1, renderer);
            lePerso->attackFrame = 7;
            lePerso->range = 450;
            lePerso->damage = 15;
            lePerso->hitRange = 130;
            break;

        case 1 :
            strcpy(tPersoA, "knight_attack.png");
            strcpy(tPersoB, "knight_block.png");
            strcpy(tPersoW, "knight_walk2.png");
            strcpy(tPersoH, "knight_hit.png");
            strcpy(tPersoD, "knight_death.png");
            strcpy(tPersoI, "knight_idle.png");

            sprite_INI(&lePerso->sprWalk, tPersoW, 0, 0, 42, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprWalk, 90);

            sprite_INI(&lePerso->sprAttack, tPersoA, 18, -20, 80, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprAttack, 120);

            sprite_INI(&lePerso->sprBlock, tPersoD, 0, 0, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprBlock, 80);

            sprite_INI(&lePerso->sprHit, tPersoH, 0, 0, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprHit, 100);

            sprite_INI(&lePerso->sprIdle, tPersoI, 0, 0, 42, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprIdle, 300);

            sprite_INI(&lePerso->sprDead, tPersoD, 0, 0, 43, 1, renderer);
            sprite_setTickInBetween(&lePerso->sprDead, 150);

            lePerso->race = human;
            lePerso->raceAttitude[undead] = -30;
            lePerso->raceAttitude[human] = 10;
            lePerso->hitRange = 90;
            lePerso->attackBoxOffset.x = 50;
            lePerso->attackBoxOffset.y = sprite_getHeight(perso_getWalkSprite(lePerso))/2;
            gameObject_INI(&lePerso->gameObject, 60, i , 300, 6, 0.70   , 1, renderer);
            lePerso->attackFrame = 5;
            lePerso->range = 600;
            lePerso->damage = 10;
            break;

    }

    lePerso->gameObject.hitBox.x = lePerso->gameObject.X;
    lePerso->gameObject.hitBox.y = lePerso->gameObject.Y;
    lePerso->gameObject.hitBox.w = sprite_getWidth(perso_getWalkSprite(lePerso))/2;
    lePerso->gameObject.hitBox.h = sprite_getHeight(perso_getWalkSprite(lePerso));

    lePerso->attackBox.x = lePerso->gameObject.X + sprite_getWidth(perso_getWalkSprite(lePerso))/2;
    lePerso->attackBox.y = lePerso->gameObject.Y + sprite_getHeight(perso_getWalkSprite(lePerso))/1.5;
    lePerso->attackBox.w = sprite_getWidth(perso_getWalkSprite(lePerso))/4;
    lePerso->attackBox.h = sprite_getHeight(perso_getWalkSprite(lePerso))/4;

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
        p->attackBox.x = p->gameObject.X + p->gameObject.boxCollider.w/2 + p->attackBoxOffset.x;
        p->attackBox.y = p->gameObject.Y + p->attackBoxOffset.y;
    }
    else
    {
        p->attackBox.x = p->gameObject.X + p->gameObject.boxCollider.w/2 - p->attackBoxOffset.x;
        p->attackBox.y = p->gameObject.Y + p->attackBoxOffset.y;
    }
    return &p->attackBox;
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

void perso_setPlayer(perso * p)
{
    p->playerControled = SDL_TRUE;
}

void perso_setHitted(perso * p, perso * p2, int dmg)
{
    // if(p->attacking != 2 && p->attacking != 4)
    if((p->attacking != 2 && p->attacking != 4) || (p->attacking == 2 && sprite_getFrame(p->sprHit) > 1))
    {
        p->attacking = 2;
        sprite_RESET(&p->sprHit);
        gameObject_addLife(&p->gameObject, -dmg);
        printf("Degat pris %i : vie = %i dmg = %i\n\n", p->gameObject.id, p->gameObject.life, dmg);
        if(p2 != NULL)
        {
            perso_setTarget(p, perso_getGameObject(p2));
            printf("ID : %i Targetting ID : %i\n", p->gameObject.id, p2->gameObject.id);
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


void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY)
{
    switch (lePerso->attacking)
    {
        case 0 :
            sprite_DRAW(&lePerso->sprWalk, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY);
        break;
        case 1 :
            sprite_DRAW(&lePerso->sprAttack, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY);
        break;
        case 2 :
            sprite_DRAW(&lePerso->sprHit, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY);
        break;
        case 3 :
            sprite_DRAW(&lePerso->sprIdle, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY);
        break;
        case 4 :
            sprite_FDRAW(&lePerso->sprDead, renderer, lePerso->gameObject.X + offsetX, lePerso->gameObject.Y + offsetY);

        break;
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

void perso_Simulate(perso * lePerso, Uint32 ticks)
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
        if(lePerso->pTarget != NULL && lePerso->playerControled == SDL_FALSE)
            {
                if(lePerso->attacking != 2 && lePerso->attacking != 1)
                    perso_setWalk(lePerso);

                if(sprite_getFrame(perso_getAttackSprite(lePerso)) < lePerso->attackFrame - 2)
                {
                    if(lePerso->pTarget->X > lePerso->gameObject.X)
                    {
                        sprite_FLIP_RIGHT(&lePerso->sprAttack);
                        sprite_FLIP_RIGHT(&lePerso->sprWalk);
                        sprite_FLIP_RIGHT(&lePerso->sprHit);
                        sprite_FLIP_RIGHT(&lePerso->sprDead);
                        sprite_FLIP_RIGHT(&lePerso->sprIdle);
                    }
                    else if(lePerso->pTarget->X < lePerso->gameObject.X)
                    {
                        sprite_FLIP_LEFT(&lePerso->sprAttack);
                        sprite_FLIP_LEFT(&lePerso->sprWalk);
                        sprite_FLIP_LEFT(&lePerso->sprHit);
                        sprite_FLIP_LEFT(&lePerso->sprIdle);
                        sprite_FLIP_LEFT(&lePerso->sprDead);
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
                    gameObject_simulate(&lePerso->gameObject, lePerso->pTarget, lePerso->path);

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
