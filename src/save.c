#include "save.h"
#include <stdio.h>
#include <stdlib.h>

// Estructura para el header del archivo
typedef struct {
    int version;           // Versión del formato de guardado
    int totalChunks;       // Número total de chunks guardados
    float playerX;         // Posición del jugador
    float playerY;
} SaveHeader;

// Estructura para guardar un chunk
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

int SaveWorld(ChunkManager *cm, Player *player) {
    FILE *file = fopen(SAVE_FILE, "wb");
    if (!file) {
        printf("Error: No se pudo crear el archivo de guardado\n");
        return 0;
    }

    // Escribir header
    SaveHeader header;
    header.version = 1;
    header.totalChunks = cm->totalGenerated;
    header.playerX = player->move.x;
    header.playerY = player->move.y;
    
    fwrite(&header, sizeof(SaveHeader), 1, file);

    // Escribir todos los chunks generados
    ChunkNode *current = cm->head;
    int savedCount = 0;
    
    while (current != NULL) {
        if (current->chunk.generated) {
            ChunkSaveData chunkData;
            chunkData.chunkX = current->chunk.chunkX;
            chunkData.chunkY = current->chunk.chunkY;
            
            // Copiar tiles
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
    printf("Mundo guardado: %d chunks, Jugador en (%.0f, %.0f)\n", 
           savedCount, header.playerX, header.playerY);
    return 1;
}

int LoadWorld(ChunkManager *cm, Player *player) {
    FILE *file = fopen(SAVE_FILE, "rb");
    if (!file) {
        printf("No se encontró archivo de guardado\n");
        return 0;
    }

    // Leer header
    SaveHeader header;
    if (fread(&header, sizeof(SaveHeader), 1, file) != 1) {
        printf("Error leyendo header del guardado\n");
        fclose(file);
        return 0;
    }

    // Verificar versión
    if (header.version != 1) {
        printf("Versión de guardado incompatible\n");
        fclose(file);
        return 0;
    }

    // Restaurar posición del jugador
    player->move.x = header.playerX;
    player->move.y = header.playerY;

    // Leer todos los chunks
    for (int i = 0; i < header.totalChunks; i++) {
        ChunkSaveData chunkData;
        if (fread(&chunkData, sizeof(ChunkSaveData), 1, file) != 1) {
            printf("Error leyendo chunk %d\n", i);
            break;
        }

        // Crear nuevo nodo de chunk
        ChunkNode *newNode = (ChunkNode*)malloc(sizeof(ChunkNode));
        newNode->chunk.chunkX = chunkData.chunkX;
        newNode->chunk.chunkY = chunkData.chunkY;
        newNode->chunk.generated = 1;
        newNode->chunk.loaded = 0; // Se cargará cuando esté cerca del jugador
        
        // Copiar tiles
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                newNode->chunk.tiles[y][x] = chunkData.tiles[y][x];
            }
        }
        
        // Agregar a la lista
        newNode->next = cm->head;
        cm->head = newNode;
        cm->totalGenerated++;
    }

    fclose(file);
    printf("Mundo cargado: %d chunks, Jugador en (%.0f, %.0f)\n", 
           header.totalChunks, header.playerX, header.playerY);
    return 1;
}