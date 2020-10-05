#include "tChar.h"
void iniPathMap(pathMap * pm, int offsetX, int offsetY, object * objects, int nbObject)
{
    pm->lenght = 0;
    pm->offset.x = offsetX;
    pm->offset.y = offsetY;
    pm->objects = objects;
    pm->nbObject = nbObject;
    pm->tchars = (tChar*)malloc(sizeof(tChar));
    settChar(pm, offsetX, offsetY, 'S');

    if(pm->tchars == NULL)
    {
        printf("Error lack of memory for pathfinding\n");
        exit(EXIT_FAILURE);
    }
}

int settChar(pathMap * pm, int x, int y, char ch)
{
    int c = findtChar(*pm, x, y);
    if(c < 0)
    {
        pm->tchars =(tChar*)realloc(pm->tchars, (pm->lenght+1)*sizeof(tChar));
        pm->tchars[pm->lenght].ch = ch;
        pm->tchars[pm->lenght].pos.x = x;
        pm->tchars[pm->lenght].pos.y = y;
        c = pm->lenght;
        pm->lenght++;
    }
    else
    {
        pm->tchars[c].ch = ch;
    }

    return c;
}

void draw_map(pathMap pm)
{
    printf("\nPathfinding :\n");
    char tab[255][255];
    for(int i = 0; i < 255; i++)
        strcpy(tab[i], "                                                             ");

    for(int i = 0; i < pm.lenght; i++)
    {
        tab[pm.tchars[i].pos.y-1][pm.tchars[i].pos.x-1] = pm.tchars[i].ch;
    }

    int i = 0;
    while(i < 30)
    {
        for(int j = 0; j < 30; j++)
            printf("%c",  tab[i][j]);
        printf("\n");
        i++;
    }
    printf("\n");
}

char gettCharPathMap(pathMap * pm, int x, int y)
{
    int c = findtChar(*pm, x, y);
    if(c == -1)
    {
        return map_readMap(pm, x, y);
    }
    else
    {
        return pm->tchars[c].ch;
    }

}

char map_readMap(pathMap * pm, int x, int y)
{
    SDL_bool passable = SDL_TRUE;
    SDL_bool found = SDL_FALSE;
    int comp = 0;
    while(comp < pm->nbObject && found == SDL_FALSE)
    {
        if(pm->objects[comp].gameObject.coord.x/TILESIZE == x && pm->objects[comp].gameObject.coord.y/TILESIZE == y)
        {
            passable = pm->objects[comp].gameObject.passable;
            found = SDL_TRUE;
        }
        comp++;
    }
    char oui;
    if(passable)
    {
        settChar(pm, x, y, '.');
        oui = '.';
    }
    else
    {
        settChar(pm, x , y, 'c');
        oui = 'c';
    }

    return oui;
}

int findtChar(pathMap pm, int x, int y)
{
    int comp = 0;
    while(comp < pm.lenght && (pm.tchars[comp].pos.x != x || pm.tchars[comp].pos.y != y))
    {
        comp++;
    }
    if(comp >= pm.lenght)
        return -1;
    return comp;
}
