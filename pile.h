#ifndef PILE_H_INCLUDED
#define PILE_H_INCLUDED

typedef struct pos2D{
    int x;
    int y;
}pos2D;

typedef struct Element Element;
struct Element
{
    pos2D nombre;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};

File *initialiser();
void enfiler(File *file, pos2D nvNombre);
pos2D defiler(File *file);
void afficherFile(File *file);

#endif // PILE_H_INCLUDED
