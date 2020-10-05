#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "object.h"
#include "boxDrawer.h"
#include "string.h"
#include "pthread.h"
#include "ground.h"
#include "item.h"
#include "IA.h"
#include <math.h>

#define CHARACPATH "characters/"

typedef enum race{human, undead}race; //KEEP UNDEAD THE LAST ONE
typedef enum status{idle, walk, attack, block, hit, farm, dead}status; //KEEP DEAD LAST
typedef enum iaState{wandering, charging, fighting, following, fleeing}iaState; //KEEP FLEEING LAST

typedef struct behavior{
    int intelligence;
}behavior;

//Contiens les différent types de perso
typedef struct scene scene;

typedef struct perso perso;
struct perso{
    perso * liege;
    race race;
    behavior behavior;
    int raceAttitude[undead+1];
    gameObject gameObject;

    sprite sprites[dead+1];
    sprite shadowSprites[dead+1];
    int attackFrame;
    vect2D attackBoxOffset;

    perso * pTarget;
    inventory * inventory;
+
    status status;
    iaState iaState;
    int sightRange;
    int hitRange;
    groundTile * focusTile;

    file * path;
    perso ** neighbours;
    perso ** inSightCharacters;
    perso * lastInFront;
    int nbNeighbours;
    int nbInSightCharacters;
    int neighboursDistance;
    int avoidDistance;
    SDL_bool inFront;

    item * items;
    item * weapon;

    void * scene;

    SDL_bool playerControled;
};
/*
typedef struct pTarget {
    perso * p;
}pTarget;*/

void perso_INI(perso * p, void * scene, SDL_Renderer * renderer, perso * model, int x, int y);
void newModel_INI(perso * lePerso, char * name, float mass, float str, SDL_bool passable, int race, int attackFrame, int range, int hitRange, sprite sprites[dead + 1], item * w, int intelligence);
void perso_setWalk(struct perso * lePerso);
void perso_setAttack(struct perso * lePerso);
void perso_setHitted(perso * p, perso * p2, item * weapon);
void perso_setDead(perso * p);
void perso_setIdle(perso * p);
void perso_setTarget(perso * lePerso, perso * target);

void perso_DRAW(perso * lePerso,  SDL_Renderer * renderer, int offsetX, int offsetY, SDL_bool debugOn);
void perso_setStatus(struct perso * p, status status, perso * p2);
void perso_MOVETO(struct perso * lePerso, int posX, int posY);
SDL_Rect * perso_getAttackBox(perso * p);
void perso_Simulate(perso * lePerso, object * objects, int nbObject, Uint32 ticks);
gameObject * perso_getGameObject(perso * p);
int perso_getHitRange(perso * p);
int collision(perso p1, perso p2);
sprite perso_getSprite(perso * p, status status);
int perso_getRaceAttitude(perso * p, race r);
int getDetecRange(perso p);
SDL_bool perso_focusTileIsInRange(perso p);
sprite * model_getSprites(perso * model);
groundTile * perso_getFocusTile(perso p);
SDL_bool perso_isMaxStamina(perso p);
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
void perso_addLife(perso * p, float life);
void perso_updateAgilities(perso * p);
SDL_bool isIAStateAgressive(perso * p);
fvect2D perso_moveToTarget(perso * lePerso, object * objects, int nbObject);
fvect2D perso_moveToPos(perso * lePerso, vect2D pos, object * objects, int nbObject);
fvect2D neighbour_getAvoidDir(perso * p, perso ** neighbours, int nbNeighbours);
vect2D findPosFromAngleDist(int angle, int dist);
int perso_getAngleAwayFromEnnemies(perso * p);
float perso_getRadAngle(perso p1, perso p2);
SDL_bool perso_friendInFront(perso * p);
SDL_bool perso_TargetInFront(perso * lePerso);
int perso_setLiege(perso * p, perso * l);
int perso_makeFollowAll(perso * l, perso * p);
void perso_blockHit(perso * p, int dmg);
int perso_makeFollow(perso * p, perso * l);
int perso_makeAttack(perso * p, perso * l);
int perso_makeWander(perso * p);
void perso_destroy(perso * p);
int perso_getAttitude(perso*p, perso *l);
void perso_changeStance(perso * p, stance stance);
SDL_bool perso_isAlive(perso p);
SDL_bool isInFront(perso * p, perso * pFront);
SDL_bool perso_isMaxLife(perso p);
SDL_bool perso_getHit(perso * p, float life);
void perso_lookAt(perso* p, perso * t);
SDL_bool perso_finishedAction(perso*p);
void perso_equipWeapon(perso * p, item * w);
SDL_bool perso_canMove(perso * p);
SDL_bool perso_isGettingAttacked(perso * p);
SDL_bool perso_EnemyAroundHit(perso* p);
SDL_bool CircleInRect(SDL_Rect * circle, SDL_Rect * rect);
SDL_bool drawThePoint(int x, int y, SDL_Rect * rect);

item * scene_findItem(scene * s, char * itemName);

#endif // PERSO_H_INCLUDED
