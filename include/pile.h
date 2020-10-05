#ifndef PILE_H_INCLUDED
#define PILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "math.h"

typedef struct vect2D{
    int x;
    int y;
}vect2D;

typedef struct fvect2D{
float x;
float y;
}fvect2D;

typedef struct Element Element;
struct Element
{
    vect2D nombre;
    Element *suivant;
};

typedef struct file file;
struct file
{
    int taille;
    Element *premier;
};

file *initialiser();
void enfiler(file *file, vect2D nvNombre);
vect2D defiler(file *file);
void afficherFile(file *file);
void viderFile(file * file);
fvect2D normalizefVect2D(fvect2D vect);
fvect2D divfVect2D(fvect2D vect, float d);
fvect2D addfVect2D(fvect2D vect, fvect2D vect2);
float getDistfVect2D(fvect2D vect, fvect2D vect2);
fvect2D multifVect(fvect2D vect, float x);
float fDenormalize(float normalized, float min, float max);
float fNormalize(float value, float min, float max);
vect2D getLastNumber(file * path);

#endif // PILE_H_INCLUDED
