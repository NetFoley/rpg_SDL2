#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#define WEAPONPATH "items/"

typedef enum weaponType{melee, range}weaponType;
typedef enum itemType{food, weapon}itemType;

typedef struct item{
    char name[255];
    itemType itemType;
    weaponType weaponType;
    int damage;
    SDL_Rect hitBox;
}item;

typedef struct inventory{
    item ** items;
    int nbrItem;
    int * qteItem;
}inventory;

void INIT_item(item* item, char name[255], itemType itemType, weaponType weaponType, int damage, int h, int w);
inventory * INIT_Inventory();
SDL_bool ADD_inventoryItem(inventory * inventory, item * item, int quantity);

#endif // WEAPON_H_INCLUDED
