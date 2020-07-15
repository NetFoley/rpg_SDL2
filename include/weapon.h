#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#define WEAPONPATH "weapons/"

typedef enum weaponType{melee, range}weaponType;

typedef struct weapon{
    char name[255];
    weaponType weaponType;
    int damage;
    SDL_Rect hitBox;


}weapon;

void INIT_weapon(weapon* weapon, char name[255], weaponType weaponType, int damage, int h, int w)
{
    strcpy(weapon->name, name);
    weapon->weaponType = weaponType;
    weapon->damage= damage;
    weapon->hitBox.h = h;
    weapon->hitBox.w = w;
    weapon->hitBox.x = 0;
    weapon->hitBox.y = 0;
}

#endif // WEAPON_H_INCLUDED
