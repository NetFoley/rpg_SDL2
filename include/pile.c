#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

file *initialiser()
{
    file *file = malloc(sizeof(file));
    file->premier = NULL;
    file->taille = 0;

    return file;
}

void enfiler(file *file, vect2D nvNombre)
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
        /* On se positionne à la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
    }
    else /* La file est vide, notre élément est le premier */
    {
        file->premier = nouveau;
    }
    file->taille++;
}

vect2D getLastNumber(file * path)
{
    vect2D nbr;
    nbr.x = 0;
    nbr.y = 0;
    if(path != NULL && path->premier != NULL)
    {
        Element * element = path->premier;
        while(element->suivant != NULL)
             element = element->suivant;

        nbr = element->nombre;
    }
    return nbr;
}

vect2D defiler(file *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    vect2D nombreDefile;
    nombreDefile.x = 0;
    nombreDefile.y = 0;
    file->taille--;

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;
        nombreDefile.x = elementDefile->nombre.x;
        nombreDefile.y = elementDefile->nombre.y;
        file->premier = elementDefile->suivant;
    }

    return nombreDefile;
}

void viderFile(file * file)
{
    if(file != NULL)
    {
        while(file->premier != NULL)
            defiler(file);
        file->premier = NULL;
    }
    file->taille = 0;
}

void afficherFile(file *file)
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

fvect2D normalizefVect2D(fvect2D vect)
{
    fvect2D newVect = vect;
    float magnitude = sqrt(newVect.x * newVect.x + newVect.y * newVect.y);
    newVect.x = newVect.x / magnitude;
    newVect.y = newVect.y / magnitude;
    return newVect;
}

fvect2D divfVect2D(fvect2D vect, float d)
{
    fvect2D newVect = vect;
    newVect.x = newVect.x / d;
    newVect.y = newVect.y / d;
    return newVect;
}

fvect2D addfVect2D(fvect2D vect, fvect2D vect2)
{
    fvect2D newVect = vect;
    newVect.x = newVect.x + vect2.x;
    newVect.y = newVect.y + vect2.y;
    return newVect;
}


float getDistfVect2D(fvect2D vect, fvect2D vect2)
{
    float dist = fabs(vect.x - vect2.x) + fabs(vect.y - vect2.y);
    return fabs(dist);
}

float fNormalize(float value, float min, float max)
{
    float normalized = (value - min)/(max - min);
    return normalized;
}

float fDenormalize(float normalized, float min, float max)
{
    float denormalized = (normalized * (max - min) + min);
    return denormalized;
}

fvect2D multifVect(fvect2D vect, float x)
{
    fvect2D newVect = vect;
    newVect.x *= x;
    newVect.y *= x;
    return newVect;
}
