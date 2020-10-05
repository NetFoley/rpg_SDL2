#ifndef scene_H_INCLUDED
#define scene_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include "perso.h"
#include "interface.h"
#include "object.h"
#include "IA.h"
#include "item.h"
#include "caracterMatching.h"
#include "ground.h"


typedef struct scene{
    SDL_Point mousePos;
    groundTile * focusTile;
    int offsetX;
    int offsetY;
	perso * persos;
	perso * modelPersos;
	object * modelObjects;
	groundTile * modelGroundTiles;
	groundTile * groundTiles;
	object * objects;
	item * items;
	perso * player;
    Uint32 timeSinceLastAction;
    int timeBetweenAction;
	int nbPerso;
	int nbModelPerso;
	int nbModelObject;
	int nbModelGroundTile;
	int nbGroundTile;
	int nbObject;
	int nbItem;
	int idPlayer;
	interface interface;

    file * events;
	caracterMatching carDef;
	char imagePath[255];
    char sMap[255];
	SDL_bool debugMode;
	SDL_Renderer * renderer;
}scene;


void scene_INI(scene * lescene, SDL_Renderer * renderer);
void scene_DRAW(scene * lescene, SDL_Renderer * renderer, SDL_Window * window);
void scene_AddSoldier(scene * lescene, int x, int y);
void scene_AddSquelette(scene * lescene, int x, int y);
void scene_AddGroundTile(scene * s, char name[255], groundState state, float growth, int x, int y);
void scene_AddObject(scene * s, char name[255], int x, int y);
void scene_getInput(scene * l, SDL_Event event);
void scene_AddWeapon(scene * l, char name[255], itemType itemType, weaponType weaponType, int damage, int h, int w);
void scene_Destroy();
void scene_readMap(scene * l, char sMap[255]);
void scene_readItems(scene * l);
void scene_readCharacters(scene * s);
void scene_readObjects(scene * s);
void scene_readGroundTiles(scene * s);
void scene_setPlayer(scene * lay, perso * p);
perso * scene_getPersoById(scene * lay, int id);
char * scene_GetImgPath(scene * scene);
perso * scene_AddModelChar(scene * s);
item * scene_findItem(scene * s, char * itemName);
perso * scene_findModelPerso(scene * s, char modelName[255]);
object * scene_findModelObject(scene * s, char modelName[255]);
groundTile * scene_findModelGroundTile(scene * s, char modelName[255]);
void refreshPaths(perso * persos, int nbPerso);
groundTile * scene_getFocusTile(groundTile * groundTiles, int nbGroundTiles, SDL_Point mousePos);


#endif // scene_H_INCLUDED
