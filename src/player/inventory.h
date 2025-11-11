#ifndef INVENTORY_H
#define INVENTORY_H

#include "raylib.h"

#define INVENTORY_COLS 9
#define INVENTORY_ROWS 5
#define INVENTORY_SIZE (INVENTORY_COLS * INVENTORY_ROWS)  // 45 slots
#define ARMOR_SLOTS 3      // Botas, Pechera, Casco
#define ACCESSORY_SLOTS 6  // Accesorios
#define SLOT_SIZE 48       // Tamaño visual de cada slot en píxeles

typedef enum {
    ITEM_NONE = 0,
    ITEM_WOOD,
    ITEM_STONE,
    ITEM_IRON,
    ITEM_GOLD,
    ITEM_DIAMOND,
    // Armaduras
    ITEM_HELMET,
    ITEM_CHESTPLATE,
    ITEM_BOOTS,
    // Accesorios
    ITEM_RING,
    ITEM_AMULET,
    ITEM_BRACELET
} ItemType;

typedef struct {
    ItemType type;
    int quantity;
} ItemStack;

typedef struct {
    ItemStack slots[INVENTORY_SIZE];     // Inventario principal (9x5)
    ItemStack armor[ARMOR_SLOTS];        // 0=Botas, 1=Pechera, 2=Casco
    ItemStack accessories[ACCESSORY_SLOTS];  // Accesorios
    int isOpen;                          // Si el inventario está abierto
    int selectedSlot;                    // Slot seleccionado (-1 = ninguno)
    ItemStack heldItem;                  // Item que estás sosteniendo con el cursor
} Inventory;

// Funciones principales
void InitInventory(Inventory *inv);
void ToggleInventory(Inventory *inv);
void UpdateInventory(Inventory *inv, Vector2 mousePos);
void DrawInventory(Inventory *inv, int screenWidth, int screenHeight);

// Funciones de items
int AddItem(Inventory *inv, ItemType type, int quantity);
int RemoveItem(Inventory *inv, ItemType type, int quantity);
int GetItemCount(Inventory *inv, ItemType type);
const char* GetItemName(ItemType type);
Color GetItemColor(ItemType type);

#endif