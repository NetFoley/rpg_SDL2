#ifndef gameObject_H_INCLUDED
#define gameObject_H_INCLUDED
#include <math.h>
#include "IA.h"
#define TILESIZE 48
#include "perso.h"

typedef struct vect2D{
int x;
int y;
}vect2D;

typedef struct gameObject{
    float X;
    float Y;
    float maxV;
    float vX;
    float vY;
    float a;
    int maxLife;
    int life;
    int id;
    SDL_Rect boxCollider;
    vect2D boxColliderOffset;
    SDL_bool movable;
    SDL_Rect hitBox;

    Uint32 timeSinceLastFrame;
}gameObject;

void gameObject_INI(gameObject * obj, int maxLife, float X , float Y, float maxV, float a, SDL_bool movable, SDL_Renderer * renderer);
int getX(gameObject * o);
int getY(gameObject * o);
int getVX(gameObject * o);
int getVY(gameObject * o);
int getMaxV(gameObject * o);
int getDA(gameObject * o);
int getA(gameObject * o);
int gameObject_getLife(gameObject * o);

void addX(gameObject * o, float X);
void addY(gameObject * o, float y);
void addVX(gameObject * o, float vX);
void addVY(gameObject * o, float vY);
void addMaxV(gameObject * o, float v);
void setA(gameObject * o, float a);
void setDA(gameObject * o, float dA);
void gameObject_addLife(gameObject * o, int l);

void gameObject_simulate(gameObject * o, gameObject * target, char sMap[50], File * path);
void gameObject_move(gameObject * o);
void gameObject_slow(float * v, float a);
void gameObject_setNewBox(gameObject * objt, int x, int y, int w, int h);
SDL_Rect * gameObject_getBoxCollider(gameObject * obj);
SDL_Rect * gameObject_getHitBox(gameObject * obj);
int attribuerID();

void gameObject_INI(gameObject * obj, int maxLife, float X , float Y, float maxV, float a, SDL_bool movable, SDL_Renderer * renderer)
{
    obj->X = X;
    obj->Y = Y;
    obj->maxV = maxV;
    obj->vX = 0;
    obj->vY = 0;
    obj->a = a;
    obj->maxLife= maxLife;
    obj->life = maxLife;
    obj->movable = movable;
    obj->boxCollider.x = X;
    obj->boxCollider.y = Y;
    obj->boxColliderOffset.x = 0;
    obj->boxColliderOffset.y = 0;
    obj->timeSinceLastFrame = SDL_GetTicks();
    obj->id = attribuerID();
    //printf("Coordonnes %f %f\nID %i\n", X ,Y, obj->id);
}


void gameObject_simulate(gameObject * o, gameObject * target, char sMap[50], File * path)
{

    if(o->movable == 1)
    {
        gameObject_getBoxCollider(o);

        if(target != NULL)
            gameObject_getBoxCollider(target);
        else
            viderFile(path);
        /**********************
              PATHFINDING
        **********************/

        //READING THE PATH
        if(path->premier != NULL)
        {
            if( path->premier->nombre.x == (int)(o->boxCollider.x+TILESIZE/2)/TILESIZE &&
                path->premier->nombre.y == (int)(o->boxCollider.y+TILESIZE/2)/TILESIZE)
            {
                viderFile(path);
            }
        }

        if(path->premier == NULL && target != NULL)
        {
            char sMap[50];
            strcpy(sMap, "map1.map");
            map_getPath(sMap,
            (int)(target->boxCollider.x+TILESIZE/2)/TILESIZE,
            (int)(target->boxCollider.y+TILESIZE/2)/TILESIZE,
            (int)     (o->boxCollider.x+TILESIZE/2)/TILESIZE,
            (int)     (o->boxCollider.y+TILESIZE/2)/TILESIZE,
            path);
        }

        //MOVING THE OBJECT
        if(path->premier != NULL)
        {
            if ((float)(o->boxCollider.x)/TILESIZE == path->premier->nombre.x)
            {
                gameObject_slow(&o->vX, o->a);
            }
            else if((float)(o->boxCollider.x)/TILESIZE < path->premier->nombre.x)
            {
                //printf("ACC :%f %f\n", o->vX, o->a);
                addVX(o,o->a);
            }
            else
            {
                // printf("-ACC :%f %f\n", o->vX, o->a);
                addVX(o,-o->a);
            }
            if ((float)(o->boxCollider.y)/TILESIZE == path->premier->nombre.y)
            {
                gameObject_slow(&o->vY, o->a);
            }
            else if((float)(o->boxCollider.y)/TILESIZE < path->premier->nombre.y)
            {
                // printf("ACC :%f %f\n", o->vY, o->a);
                addVY(o,o->a);
            }
            else
            {
                // printf("-ACC :%f %f\n", o->vY, o->a);
                addVY(o,-o->a);
            }
        }
    }

}

void gameObject_move(gameObject * o)
{
    o->X += o->vX;
    o->Y += o->vY;
}

void gameObject_slow(float * v, float a)
{
    a = 3 * a;
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
        return o->X;
    else
        return 0;
}

int getY(gameObject * o)
{
    if(o != NULL)
        return o->Y;
    else
        return 0;
}

int getVX(gameObject * o)
{
    if(o != NULL)
        return o->vX;
    else
        return 0;
}

int getVY(gameObject * o)
{
    if(o != NULL)
        return o->vY;
    else
        return 0;
}

int getMaxV(gameObject * o)
{
    if(o != NULL)
        return o->maxV;
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
    o->X = mouseX;
    o->Y = mouseY;
}


void addX(gameObject * o, float x)
{
    if(o != NULL && o->X + x > 0)
    {
        o->X += x;
    }
}

void addY(gameObject * o, float y)
{
    if(o != NULL && o->Y + y > 0)
    {
        o->X += y;
    }
}

void addVX(gameObject * o, float vX)
{
    if(o != NULL)
    {
        //Si l'accélération est vers la droite
        if(vX > 0)
        {
            if(o->vX + vX >= 0)
            {
                if(o->vX + vX <= o->maxV)
                {
                    o->vX += vX;
                }
                else
                {
                    o->vX = o->maxV;
                }
            }
            else
            {
                if(o->vX + vX*3 <= o->maxV)
                {
                    o->vX += vX*3;
                }
                else
                {
                    o->vX = o->maxV;
                }
            }
        }
        else if(vX < 0)
        {
            if(o->vX + vX <= 0)
            {
                if(o->vX + vX >= -o->maxV)
                {
                    o->vX += vX;
                }
                else
                {
                    o->vX = -o->maxV;
                }
            }
            else
            {
                if(o->vX + vX*3 >= -o->maxV)
                {
                    o->vX += vX*3;
                }
                else
                {
                    o->vX = -o->maxV;
                }
            }
        }
    }
}

void addVY(gameObject * o, float vY)
{
    if(o != NULL)
    {
        if(vY > 0)
        {
            if(o->vY + vY <= o->maxV)
            {
                o->vY += vY;
            }
            else
            {
                o->vY = o->maxV;
            }
        }
        else if(vY < 0)
        {
            if(o->vY + vY > -o->maxV)
            {
                o->vY += vY;
            }
            else
            {
                o->vY = -o->maxV;
            }
        }
    }
}

void addMaxV(gameObject * o, float v)
{
    if(o != NULL)
    {
        if(o->maxV + v > 0)
        {
            o->maxV += v;
        }
        else
        {
            o->maxV = 0;
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
    obj->boxCollider.x = obj->X + obj->boxColliderOffset.x;
    obj->boxCollider.y = obj->Y + obj->boxColliderOffset.y;
    obj->hitBox.x = obj->boxCollider.x;
    obj->hitBox.y = obj->boxCollider.y;

    return &obj->boxCollider;
}

SDL_Rect * gameObject_getHitBox(gameObject * obj)
{
    obj->boxCollider.x = obj->X + obj->boxColliderOffset.x;
    obj->boxCollider.y = obj->Y + obj->boxColliderOffset.y;
    obj->hitBox.x = obj->boxCollider.x;
    obj->hitBox.y = obj->boxCollider.y;

    return &obj->hitBox;
}

int attribuerID()
{
    static int compteur = -1;
    //printf("Compteur = %i\n", compteur);
    compteur++;
    return compteur;
}

#endif // gameObject_H_INCLUDED
