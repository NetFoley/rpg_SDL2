#ifndef gameObject_H_INCLUDED
#define gameObject_H_INCLUDED
#include <math.h>
#include "IA.h"
#define TILESIZE 48

typedef enum direction{north, east, south, west}direction; //KEEP DEAD LAST


typedef struct fvect2D{
float x;
float y;
}fvect2D;

typedef struct gameObject{
    int id;
    fvect2D coord;
    float maxS;
    fvect2D speed;
    fvect2D goalV;
    float a;
    int maxLife;
    int life;
    direction direction;
    SDL_Rect boxCollider;
    vect2D boxColliderOffset;
    SDL_bool passable;
    SDL_bool movable;

    Uint32 timeSinceLastFrame;
}gameObject;

void gameObject_INI(gameObject * obj, int maxLife, float X , float Y, float maxS, float a, SDL_bool movable, SDL_bool passable);
int getX(gameObject * o);
int getY(gameObject * o);
int getVX(gameObject * o);
int getVY(gameObject * o);
int getmaxS(gameObject * o);
int getDA(gameObject * o);
int getA(gameObject * o);
int gameObject_getLife(gameObject * o);

void addX(gameObject * o, float X);
void addY(gameObject * o, float y);
void addmaxS(gameObject * o, float v);
void setGoalV(gameObject * o, fvect2D v);
void addGoalV(gameObject * o, fvect2D v);
void updateV(gameObject * o);
void setA(gameObject * o, float a);
void setDA(gameObject * o, float dA);
void gameObject_addLife(gameObject * o, int l);
int getDistanceBetween(gameObject * p, gameObject * o);
int getNDistanceBetween(vect2D p, vect2D o);

void gameObject_simulate(gameObject * o, gameObject * target, char sMap[255], File * path);
void gameObject_move(gameObject * o);
void gameObject_slow(float * v, float acc);
void gameObject_setNewBox(gameObject * objt, int x, int y, int w, int h);
SDL_Rect * gameObject_getBoxCollider(gameObject * obj);
SDL_Rect * gameObject_getHitBox(gameObject * obj);
vect2D gameObject_getPos(gameObject o);
int attribuerID();

void gameObject_INI(gameObject * obj, int maxLife, float X , float Y, float maxS, float a, SDL_bool movable, SDL_bool passable)
{
    obj->coord.x = X;
    obj->coord.y = Y;
    obj->maxS = maxS;
    obj->speed.x = 0;
    obj->speed.y = 0;
    obj->goalV.x = 0;
    obj->goalV.y = 0;
    obj->a = a;
    obj->maxLife= maxLife;
    obj->life = maxLife;
    obj->direction = east;
    obj->movable = movable;
    obj->passable = passable;
    obj->boxCollider.x = X;
    obj->boxCollider.y = Y;
    obj->boxCollider.w = TILESIZE;
    obj->boxCollider.h = TILESIZE;
    obj->boxColliderOffset.x = 0;
    obj->boxColliderOffset.y = 0;
    obj->timeSinceLastFrame = SDL_GetTicks();
    obj->id = attribuerID();
    //printf("Coordonnes %f %f\nID %i\n", X ,Y, obj->id);
}


void gameObject_simulate(gameObject * o, gameObject * target, char sMap[255], File * path)
{
/**************************
        PATHFINDING
**************************/
    if(o != NULL && o->movable == 1)
    {
        vect2D nCible;
        gameObject_getBoxCollider(o);
        if(target != NULL)
        {
            gameObject_getBoxCollider(target);
            vect2D nTarget = gameObject_getPos(*target);
            vect2D nObject = gameObject_getPos(*o);
            if(path->premier != NULL)
            {
            //READING THE PATH
                if((path->premier->nombre.x == nObject.x &&
                    path->premier->nombre.y == nObject.y))
                {
                    defiler(path);
                }
                else if(getNDistanceBetween(nObject, nTarget) < getNDistanceBetween(path->premier->nombre, nTarget))
                {
                    viderFile(path);
                }
            }
            else
            {
                map_getPath(sMap, nTarget.x, nTarget.y, nObject.x, nObject.y, path);
            }

        }
        else
        {
            if(path->premier != NULL)
            {
                viderFile(path);
            }
        }
        if(path->premier == NULL)
        {
            nCible = gameObject_getPos(*o);
        }
        else
        {
            nCible = path->premier->nombre;
        }
        //MOVING THE OBJECT
        fvect2D goalV;
        goalV.x =  nCible.x*TILESIZE - o->boxCollider.x;
        goalV.y =  nCible.y*TILESIZE - o->boxCollider.y;
        setGoalV(o, goalV);
        // printf("ACC :%f %f\n", o->speed.y, o->a);
        updateV(o);
    }
    else
    {
        if(path != NULL && path->premier != NULL)
                viderFile(path);
    }
}

void gameObject_getDirection(gameObject * o)
{
    if(o->goalV.x != 0 || o->goalV.y != 0)
    {
        if(abs(o->goalV.x) == abs(o->goalV.y))
        {
            if(o->direction == north || o->direction == south)
            {
                if(o->goalV.x > 0)
                {
                    o->direction = east;
                }
                else
                {
                    o->direction = west;
                }
            }
            else
            {
                if(o->goalV.y < 0)
                {
                    o->direction = north;
                }
                else
                {
                    o->direction = south;
                }
            }
        }
        else
        {
            if(abs(o->goalV.x) > abs(o->goalV.y))
            {
                if(o->goalV.x > 0)
                {
                    o->direction = east;
                }
                else
                {
                    o->direction = west;
                }
            }
            else
            {
                if(o->goalV.y < 0)
                {
                    o->direction = north;
                }
                else
                {
                    o->direction = south;
                }
            }
        }
    }
}

int getDistanceBetween(gameObject * p, gameObject * o)
{
    int dist = 10000;
    if(o != NULL)
    {
        dist = abs(p->coord.x - o->coord.x) + abs(p->coord.y - o->coord.y);
    }
    return dist;
}

int getNDistanceBetween(vect2D p, vect2D o)
{
    int dist = 10000;
    dist = abs(p.x - o.x) + abs(p.y - o.y);
    return dist;
}

void setGoalV(gameObject * o, fvect2D v)
{
    o->goalV.x = v.x;
    o->goalV.y = v.y;
    if(o->goalV.x > o->maxS)
    {
        o->goalV.x = o->maxS;
    }
    else if(o->goalV.x < -o->maxS)
    {
        o->goalV.x = -o->maxS;
    }
    if(o->goalV.y > o->maxS)
    {
        o->goalV.y = o->maxS;
    }
    else if(o->goalV.y < -o->maxS)
    {
        o->goalV.y = -o->maxS;
    }
}

void setGoalVX(gameObject * o, float x)
{
    o->goalV.x = x;
    if(o->goalV.x > o->maxS)
    {
        o->goalV.x = o->maxS;
    }
    else if(o->goalV.x < -o->maxS)
    {
        o->goalV.x = -o->maxS;
    }
}
void setGoalVY(gameObject * o, float y)
{
    o->goalV.y = y;
    if(o->goalV.y > o->maxS)
    {
        o->goalV.y = o->maxS;
    }
    else if(o->goalV.y < -o->maxS)
    {
        o->goalV.y = -o->maxS;
    }
}

void addGoalV(gameObject * o, fvect2D v)
{
    o->goalV.x += v.x;
    o->goalV.y += v.y;
    if(o->goalV.x > o->maxS)
    {
        o->goalV.x = o->maxS;
    }
    else if(o->goalV.x < -o->maxS)
    {
        o->goalV.x = -o->maxS;
    }
    if(o->goalV.y > o->maxS)
    {
        o->goalV.y = o->maxS;
    }
    else if(o->goalV.y < -o->maxS)
    {
        o->goalV.y = -o->maxS;
    }

}

vect2D gameObject_getPos(gameObject o)
{
    vect2D nbr;
    nbr.x = (o.boxCollider.x + TILESIZE/2)/TILESIZE;
    nbr.y = (o.boxCollider.y + TILESIZE/2)/TILESIZE;
    return nbr;
}

void gameObject_move(gameObject * o)
{
    o->coord.x += o->speed.x;
    o->coord.y += o->speed.y;
}


void gameObject_slow(float * v, float acc)
{
    float a = 2 * acc;
    if(*(v) > 0.0)
    {
        if(*(v) - a > 0.0)
        {
            *(v) -= a;
        }
        else
            *(v) = 0.0;
    }
    else if(*(v) < 0.0)
    {
        if(*(v) + a < 0.0)
        {
            *(v) += a;
        }
        else
            *(v) = 0.0;
    }
}

int gameObject_getLife(gameObject * o)
{
    if(o != NULL)
        return o->life;
    else
        return 0;
}

void gameObject_addLife(gameObject * o, int l)
{
    if(o != NULL)
    {
        if(o->life + l > 0)
        {
            o->life += l;
        }
        else
        {
            o->life = 0;
        }
    }
}

int getX(gameObject * o)
{
    if(o != NULL)
        return o->coord.x;
    else
        return 0;
}

int getY(gameObject * o)
{
    if(o != NULL)
        return o->coord.y;
    else
        return 0;
}

int getVX(gameObject * o)
{
    if(o != NULL)
        return o->speed.x;
    else
        return 0;
}

int getVY(gameObject * o)
{
    if(o != NULL)
        return o->speed.y;
    else
        return 0;
}

int getmaxS(gameObject * o)
{
    if(o != NULL)
        return o->maxS;
    else
        return 0;
}

int getA(gameObject * o)
{
    if(o != NULL)
        return o->a;
    else
        return 0;
}

void moveToMouse(gameObject * o)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    o->coord.x = mouseX;
    o->coord.y = mouseY;
}


void addX(gameObject * o, float x)
{
    if(o != NULL && o->coord.x + x > 0)
    {
        o->coord.x += x;
    }
}

void addY(gameObject * o, float y)
{
    if(o != NULL && o->coord.y + y > 0)
    {
        o->coord.x += y;
    }
}
void updateV(gameObject * o)
{
    if(o != NULL)
    {
        gameObject_getDirection(o);
        //Si l'accélération est vers la droite
        if(o->goalV.x > o->speed.x)
        {
            if(o->speed.x + o->a >= o->goalV.x)
            {
                if(o->speed.x + o->a <= o->goalV.x)
                {
                    o->speed.x += o->a;
                }
                else
                {
                    o->speed.x = o->goalV.x;
                }
            }
            else
            {
                if(o->speed.x + o->a*2 <= o->goalV.x)
                {
                    o->speed.x += o->a*2;
                }
                else
                {
                    o->speed.x = o->goalV.x;
                }
            }
        }
        //GAUCHE
        else if(o->goalV.x  < o->speed.x)
        {
            if(o->speed.x - o->a <= 0)
            {
                if(o->speed.x - o->a >= o->goalV.x)
                {
                    o->speed.x -= o->a;
                }
                else
                {
                    o->speed.x = o->goalV.x;
                }
            }
            else
            {
                if(o->speed.x - o->a*2 >= o->goalV.x)
                {
                    o->speed.x -= o->a*2;
                }
                else
                {
                    o->speed.x = o->goalV.x;
                }
            }
        }
        //Si l'accélération est vers le bas
        if(o->goalV.y > o->speed.y)
        {
            if(o->speed.y + o->a >= 0)
            {
                if(o->speed.y + o->a <= o->goalV.y)
                {
                    o->speed.y += o->a;
                }
                else
                {
                    o->speed.y = o->goalV.y;
                }
            }
            else
            {
                if(o->speed.y + o->a*2 <= o->goalV.y)
                {
                    o->speed.y += o->a*2;
                }
                else
                {
                    o->speed.y = o->goalV.y;
                }
            }
        }
        //HAUT
        else if(o->goalV.y < o->speed.y)
        {
            if(o->speed.y - o->a <= 0)
            {
                if(o->speed.y - o->a >= o->goalV.y)
                {
                    o->speed.y -= o->a;
                }
                else
                {
                    o->speed.y = o->goalV.y;
                }
            }
            else
            {
                if(o->speed.y - o->a*2 >= o->goalV.y)
                {
                    o->speed.y -= o->a*2;
                }
                else
                {
                    o->speed.y = o->goalV.y;
                }
            }
        }
    }
}

void setA(gameObject * o, float a)
{
    if(o != NULL)
    {
        if(o->a + a > 0)
        {
            o->a += a;
        }
        else
        {
            o->a = 0;
        }
    }
}



void gameObject_setNewBox(gameObject * obj, int x, int y, int w, int h)
{
    obj->boxCollider.x = x;
    obj->boxCollider.y = y;
    obj->boxCollider.w = w;
    obj->boxCollider.h = h;
}

SDL_Rect * gameObject_getBoxCollider(gameObject * obj)
{
    if(obj != NULL)
    {
    obj->boxCollider.x = obj->coord.x + obj->boxColliderOffset.x;
    obj->boxCollider.y = obj->coord.y + obj->boxColliderOffset.y;
    return &obj->boxCollider;
    }
    return NULL;
}


int attribuerID()
{
    static int compteur = -1;
    //printf("Compteur = %i\n", compteur);
    compteur++;
    return compteur;
}

#endif // gameObject_H_INCLUDED
