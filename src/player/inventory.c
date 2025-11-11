#include "inventory.h"
#include <stdio.h>
#include <string.h>

void InitInventory(Inventory *inv) {
    // Limpiar inventario principal
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        inv->slots[i].type = ITEM_NONE;
        inv->slots[i].quantity = 0;
    }
    
    // Limpiar armadura
    for (int i = 0; i < ARMOR_SLOTS; i++) {
        inv->armor[i].type = ITEM_NONE;
        inv->armor[i].quantity = 0;
    }
    
    // Limpiar accesorios
    for (int i = 0; i < ACCESSORY_SLOTS; i++) {
        inv->accessories[i].type = ITEM_NONE;
        inv->accessories[i].quantity = 0;
    }
    
    inv->isOpen = 0;
    inv->selectedSlot = -1;
    inv->heldItem.type = ITEM_NONE;
    inv->heldItem.quantity = 0;
    
    // Agregar items de prueba
    AddItem(inv, ITEM_WOOD, 10);
    AddItem(inv, ITEM_STONE, 5);
    AddItem(inv, ITEM_IRON, 3);
}

void ToggleInventory(Inventory *inv) {
    inv->isOpen = !inv->isOpen;
}

const char* GetItemName(ItemType type) {
    switch (type) {
        case ITEM_WOOD: return "Madera";
        case ITEM_STONE: return "Piedra";
        case ITEM_IRON: return "Hierro";
        case ITEM_GOLD: return "Oro";
        case ITEM_DIAMOND: return "Diamante";
        case ITEM_HELMET: return "Casco";
        case ITEM_CHESTPLATE: return "Pechera";
        case ITEM_BOOTS: return "Botas";
        case ITEM_RING: return "Anillo";
        case ITEM_AMULET: return "Amuleto";
        case ITEM_BRACELET: return "Brazalete";
        default: return "Vacío";
    }
}

Color GetItemColor(ItemType type) {
    switch (type) {
        case ITEM_WOOD: return BROWN;
        case ITEM_STONE: return GRAY;
        case ITEM_IRON: return LIGHTGRAY;
        case ITEM_GOLD: return GOLD;
        case ITEM_DIAMOND: return SKYBLUE;
        case ITEM_HELMET: return DARKGRAY;
        case ITEM_CHESTPLATE: return DARKGRAY;
        case ITEM_BOOTS: return DARKGRAY;
        case ITEM_RING: return PURPLE;
        case ITEM_AMULET: return VIOLET;
        case ITEM_BRACELET: return MAGENTA;
        default: return DARKGRAY;
    }
}

int AddItem(Inventory *inv, ItemType type, int quantity) {
    if (type == ITEM_NONE || quantity <= 0) return 0;
    
    // Buscar slot existente con el mismo item
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv->slots[i].type == type) {
            inv->slots[i].quantity += quantity;
            return 1;
        }
    }
    
    // Buscar slot vacío
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv->slots[i].type == ITEM_NONE) {
            inv->slots[i].type = type;
            inv->slots[i].quantity = quantity;
            return 1;
        }
    }
    
    return 0; // Inventario lleno
}

int RemoveItem(Inventory *inv, ItemType type, int quantity) {
    if (type == ITEM_NONE || quantity <= 0) return 0;
    
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv->slots[i].type == type) {
            if (inv->slots[i].quantity >= quantity) {
                inv->slots[i].quantity -= quantity;
                if (inv->slots[i].quantity == 0) {
                    inv->slots[i].type = ITEM_NONE;
                }
                return 1;
            }
        }
    }
    
    return 0;
}

int GetItemCount(Inventory *inv, ItemType type) {
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv->slots[i].type == type) {
            count += inv->slots[i].quantity;
        }
    }
    return count;
}

void UpdateInventory(Inventory *inv, Vector2 mousePos) {
    if (!inv->isOpen) return;
    
    // Calcular posición del inventario centrado
    int invWidth = INVENTORY_COLS * SLOT_SIZE + 20;
    int invHeight = INVENTORY_ROWS * SLOT_SIZE + 300; // Espacio extra para armadura y accesorios
    int startX = (GetScreenWidth() - invWidth) / 2;
    int startY = (GetScreenHeight() - invHeight) / 2;
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Verificar clicks en inventario principal
        for (int row = 0; row < INVENTORY_ROWS; row++) {
            for (int col = 0; col < INVENTORY_COLS; col++) {
                int slotX = startX + 10 + col * SLOT_SIZE;
                int slotY = startY + 60 + row * SLOT_SIZE;
                
                Rectangle slotRect = {slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4};
                
                if (CheckCollisionPointRec(mousePos, slotRect)) {
                    int slotIndex = row * INVENTORY_COLS + col;
                    
                    // Si tienes un item en la mano
                    if (inv->heldItem.type != ITEM_NONE) {
                        // Intercambiar con el slot
                        ItemStack temp = inv->slots[slotIndex];
                        inv->slots[slotIndex] = inv->heldItem;
                        inv->heldItem = temp;
                    } else {
                        // Tomar el item del slot
                        if (inv->slots[slotIndex].type != ITEM_NONE) {
                            inv->heldItem = inv->slots[slotIndex];
                            inv->slots[slotIndex].type = ITEM_NONE;
                            inv->slots[slotIndex].quantity = 0;
                        }
                    }
                }
            }
        }
        
        // Verificar clicks en slots de armadura
        int armorStartY = startY + 60 + INVENTORY_ROWS * SLOT_SIZE + 20;
        const char* armorLabels[] = {"Botas", "Pechera", "Casco"};
        
        for (int i = 0; i < ARMOR_SLOTS; i++) {
            int slotX = startX + 10 + i * (SLOT_SIZE + 10);
            int slotY = armorStartY + 25;
            
            Rectangle slotRect = {slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4};
            
            if (CheckCollisionPointRec(mousePos, slotRect)) {
                if (inv->heldItem.type != ITEM_NONE) {
                    ItemStack temp = inv->armor[i];
                    inv->armor[i] = inv->heldItem;
                    inv->heldItem = temp;
                } else {
                    if (inv->armor[i].type != ITEM_NONE) {
                        inv->heldItem = inv->armor[i];
                        inv->armor[i].type = ITEM_NONE;
                        inv->armor[i].quantity = 0;
                    }
                }
            }
        }
        
        // Verificar clicks en slots de accesorios
        int accessoryStartY = armorStartY + 80;
        
        for (int i = 0; i < ACCESSORY_SLOTS; i++) {
            int col = i % 3;
            int row = i / 3;
            int slotX = startX + 10 + col * (SLOT_SIZE + 10);
            int slotY = accessoryStartY + 25 + row * (SLOT_SIZE + 10);
            
            Rectangle slotRect = {slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4};
            
            if (CheckCollisionPointRec(mousePos, slotRect)) {
                if (inv->heldItem.type != ITEM_NONE) {
                    ItemStack temp = inv->accessories[i];
                    inv->accessories[i] = inv->heldItem;
                    inv->heldItem = temp;
                } else {
                    if (inv->accessories[i].type != ITEM_NONE) {
                        inv->heldItem = inv->accessories[i];
                        inv->accessories[i].type = ITEM_NONE;
                        inv->accessories[i].quantity = 0;
                    }
                }
            }
        }
    }
}

void DrawInventory(Inventory *inv, int screenWidth, int screenHeight) {
    if (!inv->isOpen) return;
    
    // Calcular dimensiones
    int invWidth = INVENTORY_COLS * SLOT_SIZE + 20;
    int invHeight = INVENTORY_ROWS * SLOT_SIZE + 300;
    int startX = (screenWidth - invWidth) / 2;
    int startY = (screenHeight - invHeight) / 2;
    
    // Fondo semi-transparente
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});
    
    // Panel del inventario
    DrawRectangle(startX, startY, invWidth, invHeight, (Color){40, 40, 40, 255});
    DrawRectangleLines(startX, startY, invWidth, invHeight, WHITE);
    
    // Título
    DrawText("INVENTARIO", startX + 10, startY + 10, 30, WHITE);
    DrawText("Presiona E para cerrar", startX + 10, startY + 35, 16, GRAY);
    
    // Dibujar grid del inventario principal
    for (int row = 0; row < INVENTORY_ROWS; row++) {
        for (int col = 0; col < INVENTORY_COLS; col++) {
            int slotX = startX + 10 + col * SLOT_SIZE;
            int slotY = startY + 60 + row * SLOT_SIZE;
            int slotIndex = row * INVENTORY_COLS + col;
            
            // Slot background
            DrawRectangle(slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4, (Color){60, 60, 60, 255});
            DrawRectangleLines(slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4, (Color){100, 100, 100, 255});
            
            // Dibujar item si existe
            ItemStack *item = &inv->slots[slotIndex];
            if (item->type != ITEM_NONE) {
                Color itemColor = GetItemColor(item->type);
                DrawRectangle(slotX + 4, slotY + 4, SLOT_SIZE - 12, SLOT_SIZE - 12, itemColor);
                
                // Cantidad
                if (item->quantity > 1) {
                    DrawText(TextFormat("%d", item->quantity), 
                            slotX + SLOT_SIZE - 20, slotY + SLOT_SIZE - 20, 16, WHITE);
                }
            }
        }
    }
    
    // Sección de Armadura
    int armorStartY = startY + 60 + INVENTORY_ROWS * SLOT_SIZE + 20;
    DrawText("ARMADURA", startX + 10, armorStartY, 20, YELLOW);
    
    const char* armorLabels[] = {"Botas", "Pechera", "Casco"};
    for (int i = 0; i < ARMOR_SLOTS; i++) {
        int slotX = startX + 10 + i * (SLOT_SIZE + 10);
        int slotY = armorStartY + 25;
        
        DrawRectangle(slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4, (Color){60, 60, 60, 255});
        DrawRectangleLines(slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4, YELLOW);
        DrawText(armorLabels[i], slotX, slotY + SLOT_SIZE, 12, GRAY);
        
        // Dibujar item de armadura
        if (inv->armor[i].type != ITEM_NONE) {
            Color itemColor = GetItemColor(inv->armor[i].type);
            DrawRectangle(slotX + 4, slotY + 4, SLOT_SIZE - 12, SLOT_SIZE - 12, itemColor);
        }
    }
    
    // Sección de Accesorios
    int accessoryStartY = armorStartY + 80;
    DrawText("ACCESORIOS", startX + 10, accessoryStartY, 20, PURPLE);
    
    for (int i = 0; i < ACCESSORY_SLOTS; i++) {
        int col = i % 3;
        int row = i / 3;
        int slotX = startX + 10 + col * (SLOT_SIZE + 10);
        int slotY = accessoryStartY + 25 + row * (SLOT_SIZE + 10);
        
        DrawRectangle(slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4, (Color){60, 60, 60, 255});
        DrawRectangleLines(slotX, slotY, SLOT_SIZE - 4, SLOT_SIZE - 4, PURPLE);
        
        // Dibujar accesorio
        if (inv->accessories[i].type != ITEM_NONE) {
            Color itemColor = GetItemColor(inv->accessories[i].type);
            DrawRectangle(slotX + 4, slotY + 4, SLOT_SIZE - 12, SLOT_SIZE - 12, itemColor);
        }
    }
    
    // Dibujar item sostenido por el cursor
    if (inv->heldItem.type != ITEM_NONE) {
        Vector2 mousePos = GetMousePosition();
        Color itemColor = GetItemColor(inv->heldItem.type);
        DrawRectangle(mousePos.x - 20, mousePos.y - 20, 40, 40, itemColor);
        DrawRectangleLines(mousePos.x - 20, mousePos.y - 20, 40, 40, WHITE);
        
        if (inv->heldItem.quantity > 1) {
            DrawText(TextFormat("%d", inv->heldItem.quantity), 
                    mousePos.x + 10, mousePos.y + 10, 16, WHITE);
        }
    }
}