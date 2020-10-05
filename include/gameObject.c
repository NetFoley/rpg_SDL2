#include "gameObject.h"
void gameObject_INI(gameObject * obj, char * name, float mass, float str, float X, float Y, SDL_bool passable)
{
    strcpy(obj->name, name);
    obj->mass = mass;
    gameObject_setSTR(obj, str);
    obj->food = obj->maxFood;
    obj->life = obj->maxLife;
    obj->stamina = obj->maxStamina;
    obj->stance = running;

    obj->coord.x = X;
    obj->coord.y = Y;
    obj->speed.x = 0;
    obj->speed.y = 0;
    obj->goalV.x = 0;
    obj->goalV.y = 0;
    obj->direction = east;
    obj->passable = passable;

    obj->boxCollider.x = X;
    obj->boxCollider.y = Y;
    obj->boxCollider.w = TILESIZE;
    obj->boxCollider.h = TILESIZE;
    obj->boxColliderOffset.x = 0;
    obj->boxColliderOffset.y = 0;
    obj->id = attribuerID();
    //printf("Coordonnes %f %f\nID %i\n", X ,Y, obj->id);
}


void gameObject_updateAgilities(gameObject * obj)
{
    gameObject_updateAgility(obj);
    obj->maxS = 1 + obj->agility * 7;
    obj->a = obj->maxS / 6;
    obj->maxLife= obj->mass;
    obj->maxStamina = obj->mass * obj->agility;
    obj->maxFood = obj->mass;

    if(obj->agility > 0)
        obj->movable = SDL_TRUE;
    else
        obj->movable = SDL_FALSE;
}

float gameObject_updateAgility(gameObject * o)
{
    o->agility = (gameObject_updateSTR(o) / o->mass) * (o->life / o->maxLife);
    return o->agility;
}

float gameObject_getPower(gameObject * o)
{
    return gameObject_updateSTR(o) + o->mass;
}

float gameObject_simulate(gameObject * o)
{
    //Effectuer les déplacements
    if(o != NULL && o->movable == 1)
    {
        float speed = updateV(o) * o->mass / 100;
        gameObject_move(o);
        return speed;
    }
    return 0;
}

float gameObject_getSpeed(gameObject o)
{
    float speed = 0;
    if(fabs(o.speed.x) > speed)
        speed  = fabs(o.speed.x);
    if(fabs(o.speed.y) > speed)
        speed = fabs(o.speed.y);
    return speed;
}

void gameObject_addStamina(gameObject * o, float nb)
{
    if(nb >= 0)
        if(o->stamina + nb < o->maxStamina)
        {
            o->stamina += nb;
        }
        else
        {
            o->stamina = o->maxStamina;
        }
    else
        if(o->stamina + nb < -60)
            o->stamina = -60;
        else
            o->stamina += nb;
}

direction gameObject_getDirection(gameObject o)
{
    direction direction = o.direction;
    if(o.goalV.x != 0 || o.goalV.y != 0)
    {
        switch(direction)
        {
        case east:
            if(fabs(o.goalV.y) > fabs(o.goalV.x))
               {
                   if(o.goalV.y > 0)
                        direction = south;
                   if(o.goalV.y < 0)
                        direction = north;
               }
            else if(o.goalV.x < 0)
                direction = west;
            break;
        case west:
            if(fabs(o.goalV.y) > fabs(o.goalV.x))
               {
                   if(o.goalV.y > 0)
                        direction = south;
                   if(o.goalV.y < 0)
                        direction = north;
               }
            else if(o.goalV.x > 0)
                direction = east;
            break;
        case north:
            if(fabs(o.goalV.y) < fabs(o.goalV.x))
               {
                   if(o.goalV.x > 0)
                        direction = east;
                   if(o.goalV.x < 0)
                        direction = west;
               }
            else if(o.goalV.y > 0)
                direction = south;
            break;
        case south:
            if(fabs(o.goalV.y) < fabs(o.goalV.x))
               {
                   if(o.goalV.x > 0)
                        direction = east;
                   if(o.goalV.x < 0)
                        direction = west;
               }
            else if(o.goalV.y < 0)
                direction = north;
            break;
        }

    }
    return direction;
}

int getDistanceBetween(gameObject p, gameObject o)
{
    int dist = abs(p.coord.x - o.coord.x) + abs(p.coord.y - o.coord.y);
    return dist;
}

int getNDistanceBetween(vect2D p, vect2D o)
{
    int dist = 10000;
    dist = abs(p.x - o.x) + abs(p.y - o.y);
    return dist;
}

float getSpeedV(fvect2D v)
{
    return fabs(v.x) + fabs(v.y);
}

void setGoalV(gameObject * o, fvect2D v)
{
    o->goalV.x = v.x;
    o->goalV.y = v.y;
    float maxS = o->maxS;

    if(o->goalV.x > maxS)
    {
        o->goalV.x = maxS;
    }
    else if(o->goalV.x < -maxS)
    {
        o->goalV.x = -maxS;
    }
    if(o->goalV.y > maxS)
    {
        o->goalV.y = maxS;
    }
    else if(o->goalV.y < -maxS)
    {
        o->goalV.y = -maxS;
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
    fvect2D goalV = o->goalV;
    goalV.x += v.x;
    goalV.y += v.y;
    setGoalV(o, goalV);
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
    float a = acc;
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

void gameObject_addSpeed(gameObject * o, fvect2D pSpeed)
{
    o->speed.x += pSpeed.x;
    o->speed.y += pSpeed.y;
}

float gameObject_getLife(gameObject o)
{
    return o.life;
}

float gameObject_getStamina(gameObject o)
{
    return o.stamina;
}

float gameObject_getFood(gameObject o)
{
    return o.food;
}

SDL_bool gameObject_getHit(gameObject * o, float l)
{
    if(gameObject_getLastTimeSinceLastHit(*o) > 100)
    {
        o->lastTimeHit = SDL_GetTicks();
        gameObject_addLife(o, l);
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

int gameObject_getLastTimeSinceLastHit(gameObject o)
{
    return SDL_GetTicks() - o.lastTimeHit;
}

void gameObject_addLife(gameObject * o, float l)
{
    if(o != NULL)
    {
        if(o->life + l < o->maxLife)
        {
            o->life += l;
        }
        else
        {
            o->life = o->maxLife;
        }
        if(o->life < 0)
        {
            o->life = 0;
        }
    }
    gameObject_updateAgilities(o);
}

void gameObject_addFood(gameObject * o, float l)
{
    if(o != NULL)
    {
        if(o->food + l < o->maxFood)
        {
            o->food += l;
        }
        else
        {
            o->food = o->maxFood;
        }
        if(o->food < 0)
        {
            o->food = 0;
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

float updateV(gameObject * o)
{
    float speed = 0;
    if(o != NULL)
    {
        fvect2D goalV = o->goalV;
        goalV.x /= getSlowFactor(*o);
        goalV.y /= getSlowFactor(*o);
        //Si l'accélération est vers la droite
        if(goalV.x > o->speed.x)
        {
            if(o->speed.x + o->a >= goalV.x)
            {
                if(o->speed.x + o->a <= goalV.x)
                {
                    o->speed.x += o->a;
                }
                else
                {
                    o->speed.x = goalV.x;
                }
            }
            else
            {
                if(o->speed.x + o->a*2 <= goalV.x)
                {
                    o->speed.x += o->a*2;
                }
                else
                {
                    o->speed.x = goalV.x;
                }
            }
        }
        //GAUCHE
        else if(goalV.x  < o->speed.x)
        {
            if(o->speed.x - o->a <= 0)
            {
                if(o->speed.x - o->a >= goalV.x)
                {
                    o->speed.x -= o->a;
                }
                else
                {
                    o->speed.x = goalV.x;
                }
            }
            else
            {
                if(o->speed.x - o->a*2 >= goalV.x)
                {
                    o->speed.x -= o->a*2;
                }
                else
                {
                    o->speed.x = goalV.x;
                }
            }
        }
        //Si l'accélération est vers le bas
        if(goalV.y > o->speed.y)
        {
            if(o->speed.y + o->a >= 0)
            {
                if(o->speed.y + o->a <= goalV.y)
                {
                    o->speed.y += o->a;
                }
                else
                {
                    o->speed.y = goalV.y;
                }
            }
            else
            {
                if(o->speed.y + o->a*2 <= goalV.y)
                {
                    o->speed.y += o->a*2;
                }
                else
                {
                    o->speed.y = goalV.y;
                }
            }
        }
        //HAUT
        else if(goalV.y < o->speed.y)
        {
            if(o->speed.y - o->a <= 0)
            {
                if(o->speed.y - o->a >= goalV.y)
                {
                    o->speed.y -= o->a;
                }
                else
                {
                    o->speed.y = goalV.y;
                }
            }
            else
            {
                if(o->speed.y - o->a*2 >= goalV.y)
                {
                    o->speed.y -= o->a*2;
                }
                else
                {
                    o->speed.y = goalV.y;
                }
            }
        }
        if(gameObject_getSpeed(*o) >= o->maxS/2)
            o->direction = gameObject_getDirection(*o);

        speed = o->speed.x + o->speed.y;
    }
    return speed;
}

int getSlowFactor(gameObject o)
{
    return o.stance;
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

float gameObject_updateSTR(gameObject * o)
{
    gameObject_checkSTR(o);
    return o->strength;
}

float gameObject_addSTR(gameObject * o, float str)
{
    o->strength += str;
    float nbr = gameObject_checkSTR(o);
    gameObject_updateAgilities(o);
    return nbr;
}

float gameObject_getSTR(gameObject * o)
{
    return gameObject_updateSTR(o);
}

SDL_bool gameObject_setSTR(gameObject * o, float str)
{
    o->strength = str;
    SDL_bool nbr = gameObject_checkSTR(o);
    gameObject_updateAgilities(o);
    return nbr;
}

SDL_bool gameObject_checkSTR(gameObject * o)
{
    if(o->strength < o->mass/10)
    {
        o->strength = o->mass/10;
        return SDL_FALSE;
    }
    else if(o->strength > o->mass)
    {
        o->strength = o->mass;
        return SDL_FALSE;
    }
    return SDL_TRUE;
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
