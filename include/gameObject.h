#ifndef gameObject_H_INCLUDED
#define gameObject_H_INCLUDED
#include <math.h>
#define TILESIZE 48

#include <stdio.h>
#include <stdlib.h>
#include "pile.h"
#include "SDL.h"
typedef enum direction{north, east, south, west}direction; //KEEP WEST LAST
typedef enum stance{walking = 2, running = 1}stance; //KEEP DEAD LAST

typedef struct line{
SDL_Point p1;
SDL_Point p2;
}line;

typedef struct gameObject{
    char name[255];
    int id;
    fvect2D coord;
    float maxS;
    fvect2D speed;
    fvect2D goalV;
    stance stance;
    float a;
    float maxLife;
    float life;
    float maxStamina;
    float stamina;
    float food;
    float maxFood;
    float mass;
    float strength;
    float agility;
    direction direction;
    SDL_Rect boxCollider;
    vect2D boxColliderOffset;
    Uint32 lastTimeHit;
    SDL_bool passable;
    SDL_bool movable;

}gameObject;
void gameObject_INI(gameObject * obj, char * name, float mass, float str, float X, float Y, SDL_bool passable);
int getX(gameObject * o);
int getY(gameObject * o);
int getVX(gameObject * o);
int getVY(gameObject * o);
int getmaxS(gameObject * o);
int getDA(gameObject * o);
int getA(gameObject * o);
void gameObject_addSpeed(gameObject * o, fvect2D pSpeed);
float gameObject_getLife(gameObject o);
float gameObject_getFood(gameObject o);
float gameObject_getStamina(gameObject o);
float gameObject_getSpeed(gameObject o);
SDL_bool gameObject_setSTR(gameObject* o, float str);
float gameObject_addSTR(gameObject * o, float str);
SDL_bool gameObject_checkSTR(gameObject * o);
float gameObject_updateSTR(gameObject * o);
float gameObject_updateAgility(gameObject * o);
void gameObject_updateAgilities(gameObject * obj);
float gameObject_getPower(gameObject * o);
float gameObject_getSTR(gameObject * o);

void addX(gameObject * o, float X);
void addY(gameObject * o, float y);
void addmaxS(gameObject * o, float v);
void setGoalV(gameObject * o, fvect2D v);
void addGoalV(gameObject * o, fvect2D v);
float updateV(gameObject * o);
void setA(gameObject * o, float a);
void setDA(gameObject * o, float dA);
void gameObject_addLife(gameObject * o, float l);
void gameObject_addFood(gameObject * o, float l);
int getDistanceBetween(gameObject p, gameObject o);
int getNDistanceBetween(vect2D p, vect2D o);
int gameObject_getLastTimeSinceLastHit(gameObject o);
SDL_bool gameObject_getHit(gameObject * o, float l);

float gameObject_simulate(gameObject * o);
void gameObject_move(gameObject * o);
void gameObject_slow(float * v, float acc);
void gameObject_setNewBox(gameObject * objt, int x, int y, int w, int h);
SDL_Rect * gameObject_getBoxCollider(gameObject * obj);
SDL_Rect * gameObject_getHitBox(gameObject * obj);
vect2D gameObject_getPos(gameObject o);
void gameObject_addStamina(gameObject * o, float nb);
void setGoalVX(gameObject * o, float x);
float getSpeedV(fvect2D v);
void setGoalVY(gameObject * o, float y);
int getSlowFactor(gameObject o);
int attribuerID();

#endif // gameObject_H_INCLUDED
