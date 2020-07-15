#ifndef PILE_H_INCLUDED
#define PILE_H_INCLUDED

typedef struct vect2D{
    int x;
    int y;
}vect2D;

typedef struct Element Element;
struct Element
{
    vect2D nombre;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};

File *initialiser();
void enfiler(File *file, vect2D nvNombre);
vect2D defiler(File *file);
void afficherFile(File *file);
void viderFile(File * file);

#endif // PILE_H_INCLUDED
