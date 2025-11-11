#include "save.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int version;
    int seed;              // Agregar semilla
    int totalChunks;
    float playerX;
    float playerY;
} SaveHeader;

typedef struct {
    int chunkX;
    int chunkY;
    int tiles[CHUNK_SIZE][CHUNK_SIZE];
} ChunkSaveData;

int SaveExists(void) {
    FILE *file = fopen(SAVE_FILE, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int SaveWorld(ChunkManager *cm, Player *player, int seed) {
    FILE *file = fopen(SAVE_FILE, "wb");
    if (!file) {
        printf("Error: No se pudo crear el archivo de guardado\n");
        return 0;
    }

    SaveHeader header;
    header.version = 2;  // Incrementar versión por el cambio
    header.seed = seed;  // Guardar semilla
    header.totalChunks = cm->totalGenerated;
    header.playerX = player->move.x;
    header.playerY = player->move.y;
    
    fwrite(&header, sizeof(SaveHeader), 1, file);

    ChunkNode *current = cm->head;
    int savedCount = 0;
    
    while (current != NULL) {
        if (current->chunk.generated) {
            ChunkSaveData chunkData;
            chunkData.chunkX = current->chunk.chunkX;
            chunkData.chunkY = current->chunk.chunkY;
            
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    chunkData.tiles[y][x] = current->chunk.tiles[y][x];
                }
            }
            
            fwrite(&chunkData, sizeof(ChunkSaveData), 1, file);
            savedCount++;
        }
        current = current->next;
    }

    fclose(file);
    printf("Mundo guardado: %d chunks, Semilla: %d, Jugador en (%.0f, %.0f)\n", 
           savedCount, seed, header.playerX, header.playerY);
    return 1;
}

int LoadWorld(ChunkManager *cm, Player *player, int *seed) {
    FILE *file = fopen(SAVE_FILE, "rb");
    if (!file) {
        printf("No se encontró archivo de guardado\n");
        return 0;
    }

    SaveHeader header;
    if (fread(&header, sizeof(SaveHeader), 1, file) != 1) {
        printf("Error leyendo header del guardado\n");
        fclose(file);
        return 0;
    }

    // Verificar versión (aceptar versión 1 y 2)
    if (header.version != 1 && header.version != 2) {
        printf("Versión de guardado incompatible\n");
        fclose(file);
        return 0;
    }

    // Cargar semilla (si es versión 2)
    if (header.version == 2) {
        *seed = header.seed;
    } else {
        // Para guardados antiguos, usar semilla aleatoria
        *seed = rand();
        printf("Advertencia: Guardado antiguo sin semilla, usando semilla aleatoria\n");
    }

    player->move.x = header.playerX;
    player->move.y = header.playerY;

    for (int i = 0; i < header.totalChunks; i++) {
        ChunkSaveData chunkData;
        if (fread(&chunkData, sizeof(ChunkSaveData), 1, file) != 1) {
            printf("Error leyendo chunk %d\n", i);
            break;
        }

        ChunkNode *newNode = (ChunkNode*)malloc(sizeof(ChunkNode));
        newNode->chunk.chunkX = chunkData.chunkX;
        newNode->chunk.chunkY = chunkData.chunkY;
        newNode->chunk.generated = 1;
        newNode->chunk.loaded = 0;
        
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                newNode->chunk.tiles[y][x] = chunkData.tiles[y][x];
            }
        }
        
        newNode->next = cm->head;
        cm->head = newNode;
        cm->totalGenerated++;
    }

    fclose(file);
    printf("Mundo cargado: %d chunks, Semilla: %d, Jugador en (%.0f, %.0f)\n", 
           header.totalChunks, *seed, header.playerX, header.playerY);
    return 1;
}