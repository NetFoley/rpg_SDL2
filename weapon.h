#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

typedef enum type{melee, range}type;

typedef struct weapon{
    char * name;
    type type;
    int damage;
    SDL_Rect hitBox;

}weapon;

void INIT_weapon(weapon* weapon, char * name, type type, int damage, int h, int w)
{
    weapon->name = (char*)malloc(sizeof(name));
    weapon->name = name;
    weapon->type = type;
    weapon->damage= damage;
    weapon->hitBox.h = h;
    weapon->hitBox.w = w;
    weapon->hitBox.x = 0;
    weapon->hitBox.y = 0;
}

#endif // WEAPON_H_INCLUDED
