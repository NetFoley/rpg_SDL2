#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

File *initialiser()
{
    File *file = malloc(sizeof(file));
    file->premier = NULL;

    return file;
}

void enfiler(File *file, pos2D nvNombre)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->nombre = nvNombre;
    nouveau->suivant = NULL;

    if (file->premier != NULL) /* La file n'est pas vide */
    {
        /* On se positionne � la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La file est vide, notre �l�ment est le premier */
    {
        file->premier = nouveau;
    }
}


pos2D defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    pos2D nombreDefile;
    nombreDefile.x = 0;
    nombreDefile.y = 0;

    /* On v�rifie s'il y a quelque chose � d�filer */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;
        nombreDefile.x = elementDefile->nombre.x;
        nombreDefile.y = elementDefile->nombre.y;
        file->premier = elementDefile->suivant;
        free(elementDefile);
    }

    return nombreDefile;
}

void viderFile(File * file)
{
    if(file != NULL)
        while(file->premier != NULL)
            defiler(file);
}

void afficherFile(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *element = file->premier;

    printf("Affichage liste\n");
    while (element != NULL)
    {
        printf("%d %d\n", element->nombre.x, element->nombre.y);
        element = element->suivant;
    }

    printf("\n");
}

