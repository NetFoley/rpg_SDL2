#include "IA.h"
int map_getPath(object * objects, int nbObject, int xS, int yS, int xE, int yE, file * path)
{
    pathMap cPathMap;
    iniPathMap(&cPathMap, xS, yS, objects, nbObject);

    frontier frt;
    iniFrontier(&frt);

    frt.coord[0].x = xS;
    frt.coord[0].y = yS;
    map_putFrontier(&cPathMap, &frt, frt.coord[0].x, frt.coord[0].y);

    SDL_bool stop = SDL_FALSE;

    /**********************************************
        GENERATING PATH TO TARGET
    **********************************************/
    int comp = 0;
    vect2D posS;
    posS.x = xS;
    posS.y = yS;
    vect2D posE;
    posE.x = xE;
    posE.y = yE;
    int maxIterations = 10 + getNDistanceBetween(posE, posS) * 10;
    while(frt.taille > 0 && stop != SDL_TRUE)
    {
        vect2D current = map_getClosestFrontier(&frt, posE);

        if(gettCharPathMap(&cPathMap, current.x+1, current.y) == '.')
        {
            settChar(&cPathMap, current.x+1, current.y, 'G');
            map_putFrontier(&cPathMap, &frt, current.x+1, current.y);
        }
        if(gettCharPathMap(&cPathMap, current.x-1, current.y) == '.')
        {
            settChar(&cPathMap, current.x-1, current.y, 'D');
            map_putFrontier(&cPathMap, &frt, current.x-1, current.y);
        }
        if(gettCharPathMap(&cPathMap, current.x, current.y+1) == '.')
        {
            settChar(&cPathMap, current.x, current.y+1, 'H');
            map_putFrontier(&cPathMap, &frt, current.x, current.y+1);
        }

        if(gettCharPathMap(&cPathMap, current.x, current.y-1) == '.')
        {
            settChar(&cPathMap, current.x, current.y-1, 'B');
            map_putFrontier(&cPathMap, &frt, current.x, current.y-1);
        }
        if((current.x == xE && current.y == yE) || comp > maxIterations)
            stop = SDL_TRUE;

        comp++;

    }
    //freeFrontier(&frt);


    /**********************************************
        GENERATING PATH OF CHARACTER TO TARGET
    **********************************************/
    //draw_map(cPathMap);

    if (path == NULL)
    {
        exit(EXIT_FAILURE);
    }

    while(path->premier != NULL)
    {
        defiler(path);
    }

    vect2D nvPos;
    nvPos.x = xE;
    nvPos.y = yE;
    stop = SDL_FALSE;
    do
    {
        switch(gettCharPathMap(&cPathMap, nvPos.x, nvPos.y))
        {
        case 'B':
            nvPos.y = nvPos.y + 1;
            break;
        case 'H':
            nvPos.y = nvPos.y - 1;
            break;
        case 'D':
            nvPos.x = nvPos.x + 1;
            break;
        case 'G':
            nvPos.x = nvPos.x - 1;
            break;
        default:
            stop = SDL_TRUE;
        }
        enfiler(path, nvPos);
        //afficherPile(path);
        //printf("s x %i y %i e x %i y %i %c\n", xS, yS, xE, yE, cPathMap->cDirMap[nvPos.y][nvPos.x]);

    }while((path->premier->nombre.x != 0 || path->premier->nombre.y != 0) && stop == SDL_FALSE);
    //defiler(path);
    return 0;
}

int getMapHeight(char sMap[255])
{
    int height = 0;
    FILE * pFile;
    pFile = fopen(sMap, "r");
    char a='a';
    if (pFile != NULL)
    {
        while(a != EOF)
        {
            a = fgetc(pFile);
            if(a == '\n')
            {
                height++;
            }
        }
        fclose(pFile);
    }
    return height;
}

fvect2D getPosRatioFromAngle(float angle)
{
    fvect2D posRatio;
    float radAngle = angle /180 *PI;
    posRatio.x = cos(radAngle);
    posRatio.y = sin(radAngle);

    return posRatio;
}


float getRadAngle(float pos1, float pos2)
{
    float nbr = atan2(pos1, pos2)*180/PI;
    return nbr;
}


vect2D findPosFromAngleDist(int angle, int dist)
{
    fvect2D fDirection = getPosRatioFromAngle(angle);
    fDirection.x *= dist;
    fDirection.y *= dist;

    vect2D nDirection;
    nDirection.x = round(fDirection.x);
    nDirection.y = round(fDirection.y);

    return nDirection;
}


direction getAngleDirection(float angle)
{
    direction dir = north;
    if(angle < 45 && angle > -45)
        dir = east;
    else if(angle > 135 || angle <  -135)
        dir = west;
    else if(angle > 45 && angle < 135)
        dir = north;
    else if(angle > -135 && angle < -45)
        dir = south;

    return dir;
}

int getMapWidth(char sMap[255])
{
    int width = 0;
    FILE * pFile;
    pFile = fopen(sMap, "r");
    if (pFile != NULL)
    {
        char a='a';
        while(a != EOF && a != '\n')
        {
            a = fgetc(pFile);
            width++;
        }
        fclose(pFile);
    }
    return width;
}



//DEFINIR LES CASES TRAVERSABLES
//EXPLORER TOUTE LA MAP FRONTIERE PAR FRONTIERE AVEC EARLY EXIT
//DEFINIR LA DIRECTION A SUIVRE DE L'OBJECTIF SUR CHAQUE CASE POUR SE RAPPROCHER DU PERSO
//CREER UN PATH
//AJOUTER AU PATH CHAQUE ETAPE DE L'OBJECTIF AU PERSO
//INVERSER LE PATH
//SUIVRE LES CASES UNE A UNE
//FIN


void iniFrontier(frontier * frt)
{
    frt->taille = 0;
    frt->coord = (vect2D*)malloc(frt->taille+1 * sizeof(vect2D));
}

void map_putFrontier(pathMap * cPathMap, frontier * frt, int x, int y)
{
    frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
    frt->coord[frt->taille].x = x;
    frt->coord[frt->taille].y = y;
    frt->taille++;
    /*
    if(gettCharPathMap(cPathMap, x+1, y) == '.' && !(x+1 == frt->coord[frt->taille-1].x && y == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x+1;
        frt->coord[frt->taille].y = y;
        frt->taille++;
    }
    if(gettCharPathMap(cPathMap, x-1, y) == '.' && !(x-1 == frt->coord[frt->taille-1].x && y == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x-1;
        frt->coord[frt->taille].y = y;
        frt->taille++;
    }
    if(gettCharPathMap(cPathMap, x, y+1) == '.' && !(x == frt->coord[frt->taille-1].x && y+1 == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x;
        frt->coord[frt->taille].y = y+1;
        frt->taille++;
    }
    if(gettCharPathMap(cPathMap, x, y-1) == '.' && !(x == frt->coord[frt->taille-1].x && y-1 == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x;
        frt->coord[frt->taille].y = y-1;
        frt->taille++;
    }
    */
}

vect2D map_getFrontier(frontier * frt)
{
    vect2D p = frt->coord[0];
    frt->taille--;
    for(int i = 0; i < frt->taille; i++)
    {
        frt->coord[i] = frt->coord[i+1];
    }
    frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
    return p;
}

vect2D map_getClosestFrontier(frontier * frt, vect2D pos)
{
    int nClosestFrontier = 0;
    for(int i = 0; i < frt->taille; i++)
    {
        if(getNDistanceBetween(pos, frt->coord[i]) < getNDistanceBetween(pos, frt->coord[nClosestFrontier]))
        {
            nClosestFrontier = i;
        }
    }
    vect2D p = frt->coord[nClosestFrontier];
    frt->taille--;
    for(int i = nClosestFrontier; i < frt->taille; i++)
    {
        frt->coord[i] = frt->coord[i+1];
    }
    frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
    return p;
}

void freeFrontier(frontier * frt)
{
    free(frt->coord);
}
/*
void addXPathMap(pathMap * pm)
{
    pm->tailleX++;
    (char*)realloc(pm->cDirMap[pm->tailleY], (pm->tailleX+1)*sizeof(char));
}

void addYPathMap(pathMap * pm)
{
    pm->tailleY++;
    (char**)realloc(pm->cDirMap, (pm->tailleY+1)*sizeof(char*));

    pm->cDirMap[pm->tailleY-1] = (char*)malloc((pm->tailleX+1)*sizeof(char));
    pm->cDirMap[pm->tailleY-1][0] = '\0';
}*/
