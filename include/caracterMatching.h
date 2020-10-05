#ifndef CARACTERMATCHING_H_INCLUDED
#define CARACTERMATCHING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define CARPATH "maps/"

typedef struct caracterMatching{
    int nbr;
    char * caracters;
    char ** names;
}caracterMatching;

void match_ini(caracterMatching * cm);
void match_file(caracterMatching * cm, char name[255], char car);
void carMatch_destroy(caracterMatching * cm);

#endif // CARACTERMATCHING_H_INCLUDED
