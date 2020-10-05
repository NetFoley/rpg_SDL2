#include "item.h"
void INIT_item(item* item, char name[255], itemType itemType, weaponType weaponType, int damage, int h, int w)
{
    strcpy(item->name, name);
    item->itemType = itemType;
    item->weaponType = weaponType;
    item->damage= damage;
    item->hitBox.h = h;
    item->hitBox.w = w;
    item->hitBox.x = 0;
    item->hitBox.y = 0;
    switch(item->itemType)
    {
    case food:
    printf("Initialised food : %s\n", name);

        break;
    case weapon:
    printf("Initialised weapon : %s\n", name);

        break;
    }
}

inventory * INIT_Inventory()
{
    inventory * inv = (inventory*)malloc(sizeof(inventory));
    inv->nbrItem = 0;
    inv->qteItem = (int*)malloc(sizeof(int));
    inv->qteItem[0] = 0;
    inv->items = (item**)malloc(sizeof(item*));
    inv->items[0] = NULL;

    return inv;
}

SDL_bool ADD_inventoryItem(inventory * inventory, item * theItem, int quantity)
{
    if(theItem == NULL || inventory == NULL)
    {
        return SDL_FALSE;
    }
    SDL_bool itemExist = SDL_FALSE;
    int itemID = 0;
    for(int i = 0; i < inventory->nbrItem; i++)
    {
        if(inventory->items[i] == theItem)
        {
            itemID = i;
            itemExist = SDL_TRUE;
        }
    }
    if(itemExist)
    {
        if(quantity < 0 && inventory->qteItem[itemID] < abs(quantity))
            return SDL_FALSE;
        else
        {
            inventory->qteItem[itemID] += quantity;
            printf("Added %i %s\n", quantity, inventory->items[itemID]->name);
            if(inventory->qteItem[itemID] <= 0)
            {
                inventory->items[itemID] = NULL;
            }
        }
    }
    else
    {
        if(quantity > 0)
        {
            inventory->items = (item**)realloc(inventory->items, sizeof(item*) * (inventory->nbrItem+1));
            inventory->qteItem = (int*)realloc(inventory->qteItem, sizeof(int) * (inventory->nbrItem+1));
            inventory->items[inventory->nbrItem] = theItem;
            inventory->qteItem[inventory->nbrItem] = quantity;
            printf("Added %i %s\n", quantity, inventory->items[inventory->nbrItem]->name);
            inventory->nbrItem++;
        }
        else
        {
            printf("Need %s\n", theItem->name);
            return SDL_FALSE;
        }
    }

    return SDL_TRUE;
}
