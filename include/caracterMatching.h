#ifndef CARACTERMATCHING_H_INCLUDED
#define CARACTERMATCHING_H_INCLUDED

#include <stdio.h>

#define CARPATH "maps/"

typedef struct caracterMatching{
    int nbr;
    char * caracters;
    char ** names;
}caracterMatching;

void match_ini(caracterMatching * cm)
{
    cm->nbr = 0;
    cm->caracters = (char*)malloc(sizeof(char));
    cm->names = (char**)malloc(sizeof(char*));
}

void match_file(caracterMatching * cm, char name[255], char car)
{
    cm->caracters =(char*)realloc(cm->caracters, (cm->nbr+1)*(sizeof(char)));
    cm->names =(char**)realloc(cm->names, (cm->nbr+1)*(sizeof(char*)));
    cm->names[cm->nbr] = (char*)malloc(255*sizeof(char));
    cm->caracters[cm->nbr] = car;
    strcpy(cm->names[cm->nbr], name);
    cm->nbr++;

}

void carMatch_destroy(caracterMatching * cm)
{
    for(int i = 0; i < cm->nbr; i++)
    {
        free(cm->names[i]);
    }
    free(cm->caracters);
}
#endif // CARACTERMATCHING_H_INCLUDED
