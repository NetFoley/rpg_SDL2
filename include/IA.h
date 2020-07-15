#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED
#include "pile.h"

#define MAPPATH "maps/"

typedef struct frontier{
    vect2D * coord;
    int taille;
}frontier;

typedef struct pathMap{
    int tailleX;
    int tailleY;
    char **cDirMap;
}pathMap;

int map_getPath(char sMap[255], int xS, int yS, int xE, int yE, File * path);
void map_readMap(char sMap[255], pathMap * pm);
void map_putFrontier(pathMap * cPathMap, frontier * frontier, int x, int y);
vect2D map_getFrontier(frontier * f);

void iniPathMap(pathMap * pm, int width, int height);
void freePathMap(pathMap * pm);
/*
void addXPathMap(pathMap * pm);
void addYPathMap(pathMap * pm);
*/
void setPathMap(pathMap * pm, int x, int y, char c);
char getPathMap(pathMap * pm, int x, int y);

int getMapHeight(char sMap[255]);
int getMapWidth(char sMap[255]);

void iniFrontier(frontier * frt);
void freeFrontier(frontier * frt);

int map_getPath(char sSMap[255], int xS, int yS, int xE, int yE, File * path)
{
    pathMap cPathMap;
    int width = getMapWidth(sSMap);
    int height = getMapHeight(sSMap);
    if(xS < 0 || xS > width ||
       yS < 0 || yS > height)
    {
        printf("\nErreur de coordonnes x %i y %i pour un max de x %i y %i\n", xS, yS, width, height);
        return -1;
    }
    if(xE < 0 || xE > width ||
       yE < 0 || yE > height)
    {
        printf("\nErreur de coordonnes x %i y %i pour un max de x %i y %i\n", xE, yE, width, height);
        return -1;
    }

    iniPathMap(&cPathMap, width, height);
    map_readMap(sSMap, &cPathMap);

    frontier frt;
    iniFrontier(&frt);

    //setPathMap(&cPathMap, xS, yS, 'S');
    cPathMap.cDirMap[yS][xS] = 'S';
    frt.coord[0].x = xS;
    frt.coord[0].y = yS;
    map_putFrontier(&cPathMap, &frt, frt.coord[0].x, frt.coord[0].y);

    SDL_bool stop = SDL_FALSE;

    while(frt.taille > 0 && stop != SDL_TRUE)
    {
        vect2D current = map_getFrontier(&frt);

        if(getPathMap(&cPathMap, current.x+1, current.y) == '.')
        {
            cPathMap.cDirMap[current.y][current.x+1] = 'G';
            map_putFrontier(&cPathMap, &frt, current.x+1, current.y);
        }
        if(getPathMap(&cPathMap, current.x-1, current.y) == '.')
        {
            cPathMap.cDirMap[current.y][current.x-1] = 'D';
            map_putFrontier(&cPathMap, &frt, current.x-1, current.y);
        }
        if(getPathMap(&cPathMap, current.x, current.y+1) == '.')
        {
            cPathMap.cDirMap[current.y+1][current.x]=  'H';
            map_putFrontier(&cPathMap, &frt, current.x, current.y+1);
        }

        if(getPathMap(&cPathMap, current.x, current.y-1) == '.')
        {
            cPathMap.cDirMap[current.y-1][current.x] = 'B';
            map_putFrontier(&cPathMap, &frt, current.x, current.y-1);
        }

    }
    //freeFrontier(&frt);

    if(stop == SDL_TRUE)
        printf("\Echec de la creation du pathfinding\n");

    /**********************************************
    REGLER LE PATHFINDING
    **********************************************/
    /*
    printf("\nPathfinding :\n");
    int i = 0, j = 0;
    while(cPathMap->cDirMap[j][i] != '\0')
    {
        if(cPathMap->cDirMap[j][i] == '\n')
        {
            j++;
            i = 0;
            printf("\n");
        }
        else
        {
            printf("%c", cPathMap->cDirMap[j][i]);
            i++;
        }
    }
    printf("\n");
    */

    if (path == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *elementDepile;
    while(path != NULL && path->premier != NULL)
    {
        elementDepile = path->premier;
        path->premier = elementDepile->suivant;
        free(elementDepile);
    }

    vect2D nvPos;
    nvPos.x = xE;
    nvPos.y = yE;
    stop = SDL_FALSE;
    do
    {
        switch(cPathMap.cDirMap[nvPos.y][nvPos.x])
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

    }while((path->premier->nombre.x != xS || path->premier->nombre.y != yS) && stop == SDL_FALSE);
    defiler(path);
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


void iniPathMap(pathMap * pm, int width, int height)
{
    pm->tailleX = width+1;
    pm->tailleY = height+1;
    pm->cDirMap = (char**)malloc((pm->tailleY+1)*sizeof(char*));

    for(int i = 0; i < pm->tailleY+1; i++)
    {
        pm->cDirMap[i] = malloc((pm->tailleX+1)*sizeof(char));
        pm->cDirMap[i][0] = '\0';
    }

    if(pm->cDirMap == NULL)
    {
        printf("Error lack of memory of pathfinding\n");
        exit(EXIT_FAILURE);
    }
}

char getPathMap(pathMap * pm, int x, int y)
{
    if(pm != NULL)
    {
        if(pm->tailleX >= x && pm->tailleY >= y && x >= 0 && y >= 0)
        {
            if(pm->cDirMap[y] != NULL)
            {
                return pm->cDirMap[y][x];
            }
            else
            {
                printf("Erreur case non valide X : %i Y : %i\n", x , y);
                return 'E';
            }
        }
        else
        {
            printf("case non allouee X : %i Y : %i\n", x , y);
            return 'E';
        }
    }
    else
    {
        printf("Erreur map null\n");
        return 'E';
    }
}

void setPathMap(pathMap * pm, int x, int y, char c)
{
    if(pm != NULL)
    {
        if(pm->tailleX >= x && pm->tailleY >= y && x >= 0 && y >= 0)
        {
            if(pm->cDirMap[y] != NULL)
            {
                pm->cDirMap[y][x] = c;
                pm->cDirMap[y][x+1] = '\0';
            }
            else
                printf("Erreur case non valide X : %i Y : %i\n", x , y);
        }
        else
            printf("case non allouee X : %i Y : %i\n", x , y);
    }
    else
        printf("Erreur map null\n");
}

void freePathMap(pathMap * pm)
{
    for(int i = 0; i < pm->tailleY; i++)
    {
        free(pm->cDirMap[i]);
    }
    free(pm);
}

//DEFINIR LES CASES TRAVERSABLES
//EXPLORER TOUTE LA MAP FRONTIERE PAR FRONTIERE AVEC EARLY EXIT
//DEFINIR LA DIRECTION A SUIVRE DE L'OBJECTIF SUR CHAQUE CASE POUR SE RAPPROCHER DU PERSO
//CREER UN PATH
//AJOUTER AU PATH CHAQUE ETAPE DE L'OBJECTIF AU PERSO
//INVERSER LE PATH
//SUIVRE LES CASES UNE A UNE
//FIN


void map_readMap(char sMap[255], pathMap * pm)
{
    FILE * pFile;
    pFile = fopen(sMap, "r");
    if (pFile != NULL)
    {
        char a='#';
        int i = 0, j = 0;
        while(a != EOF)
        {
            a = fgetc(pFile);
            switch (a)
            {
            case 'c' :
                setPathMap(pm, i, j, '#');
                break;
            case '\n' :
                setPathMap(pm, i, j, '\n');
                j++;
                i = -1;
                break;
            case EOF:
                break;
            default :
                setPathMap(pm, i, j, '.');
            }
            i++;
        }
        fclose(pFile);
    }
}

void iniFrontier(frontier * frt)
{
    frt->taille = 1;
    frt->coord = (vect2D*)malloc(frt->taille * sizeof(vect2D));
}

void map_putFrontier(pathMap * cPathMap, frontier * frt, int x, int y)
{
    if(getPathMap(cPathMap, x+1, y) == '.' && !(x+1 == frt->coord[frt->taille-1].x && y == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x+1;
        frt->coord[frt->taille].y = y;
        frt->taille++;
    }
    if(getPathMap(cPathMap, x-1, y) == '.' && !(x-1 == frt->coord[frt->taille-1].x && y == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x-1;
        frt->coord[frt->taille].y = y;
        frt->taille++;
    }
    if(getPathMap(cPathMap, x, y+1) == '.' && !(x == frt->coord[frt->taille-1].x && y+1 == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x;
        frt->coord[frt->taille].y = y+1;
        frt->taille++;
    }
    if(getPathMap(cPathMap, x, y-1) == '.' && !(x == frt->coord[frt->taille-1].x && y-1 == frt->coord[frt->taille-1].y))
    {
        frt->coord =(vect2D*)realloc(frt->coord, (frt->taille+1)*(sizeof(vect2D)));
        frt->coord[frt->taille].x = x;
        frt->coord[frt->taille].y = y-1;
        frt->taille++;
    }
}

vect2D map_getFrontier(frontier * frt)
{
    vect2D p = frt->coord[0];
    frt->taille--;
    for(int i = 0; i < frt->taille; i++)
    {
        frt->coord[i] = frt->coord[i+1];
    }
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

#endif // IA_H_INCLUDED
