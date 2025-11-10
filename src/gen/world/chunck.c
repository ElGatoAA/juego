#include "chunk.h"
#include "noise.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define SCALE 0.02f
#define OCTAVES 4
#define PERSISTENCE 0.5f
#define MAX_LOADED_CHUNKS 50

static float octaveNoise(float x, float y) {
    float total = 0;
    float amplitude = 1;
    float frequency = SCALE;
    float maxValue = 0;

    for (int i = 0; i < OCTAVES; i++) {
        total += PerlinNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= PERSISTENCE;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

void InitChunkManager(ChunkManager *cm, int renderDistance) {
    cm->head = NULL;
    cm->loadedCount = 0;
    cm->totalGenerated = 0;
    cm->renderDistance = renderDistance;
}

static ChunkNode* FindChunkNode(ChunkManager *cm, int chunkX, int chunkY) {
    ChunkNode *current = cm->head;
    while (current != NULL) {
        if (current->chunk.chunkX == chunkX && current->chunk.chunkY == chunkY) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void GenerateChunkTerrain(Chunk *chunk, int chunkX, int chunkY) {
    int offsetX = chunkX * CHUNK_SIZE;
    int offsetY = chunkY * CHUNK_SIZE;

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float worldX = offsetX + x;
            float worldY = offsetY + y;
            
            float n = octaveNoise(worldX, worldY);

            if (n < 0.25f) chunk->tiles[y][x] = 4;
            else if (n < 0.4f) chunk->tiles[y][x] = 3;
            else if (n < 0.48f) chunk->tiles[y][x] = 2;
            else if (n < 0.7f) chunk->tiles[y][x] = 1;
            else chunk->tiles[y][x] = 0;
        }
    }
    
    chunk->chunkX = chunkX;
    chunk->chunkY = chunkY;
    chunk->generated = 1;
    chunk->loaded = 1;
}

static ChunkNode* CreateChunk(ChunkManager *cm, int chunkX, int chunkY) {
    ChunkNode *newNode = (ChunkNode*)malloc(sizeof(ChunkNode));
    
    GenerateChunkTerrain(&newNode->chunk, chunkX, chunkY);
    
    newNode->next = cm->head;
    cm->head = newNode;
    cm->loadedCount++;
    cm->totalGenerated++;
    
    return newNode;
}

static void UnloadChunk(Chunk *chunk) {
    chunk->loaded = 0;
}

static void LoadChunk(Chunk *chunk) {
    chunk->loaded = 1;
}

static void UnloadDistantChunks(ChunkManager *cm, int playerChunkX, int playerChunkY) {
    int unloadDistance = cm->renderDistance + 3;
    
    ChunkNode *current = cm->head;
    while (current != NULL) {
        if (current->chunk.loaded) {
            int dx = abs(current->chunk.chunkX - playerChunkX);
            int dy = abs(current->chunk.chunkY - playerChunkY);
            
            if (dx > unloadDistance || dy > unloadDistance) {
                UnloadChunk(&current->chunk);
                cm->loadedCount--;
            }
        }
        current = current->next;
    }
}

void UpdateChunks(ChunkManager *cm, float playerX, float playerY) {
    int playerChunkX = (int)floor(playerX / CHUNK_PIXEL_SIZE);
    int playerChunkY = (int)floor(playerY / CHUNK_PIXEL_SIZE);

    if (cm->loadedCount > MAX_LOADED_CHUNKS) {
        UnloadDistantChunks(cm, playerChunkX, playerChunkY);
    }

    for (int dy = -cm->renderDistance; dy <= cm->renderDistance; dy++) {
        for (int dx = -cm->renderDistance; dx <= cm->renderDistance; dx++) {
            int chunkX = playerChunkX + dx;
            int chunkY = playerChunkY + dy;

            ChunkNode *node = FindChunkNode(cm, chunkX, chunkY);
            
            if (node == NULL) {
                // SOLO generar si no existe en absoluto
                CreateChunk(cm, chunkX, chunkY);
            } else if (!node->chunk.loaded) {
                // Si existe pero está descargado, solo cargarlo (NO regenerar)
                LoadChunk(&node->chunk);
                cm->loadedCount++;
            }
        }
    }
}

void DrawChunks(ChunkManager *cm, Texture2D tiles[5], Camera2D *camera, int screenWidth, int screenHeight) {
    ChunkNode *current = cm->head;
    
    // Calcular área visible de la cámara
    float zoom = camera->zoom;
    float camLeft = camera->target.x - (screenWidth / 2.0f) / zoom;
    float camRight = camera->target.x + (screenWidth / 2.0f) / zoom;
    float camTop = camera->target.y - (screenHeight / 2.0f) / zoom;
    float camBottom = camera->target.y + (screenHeight / 2.0f) / zoom;
    
    while (current != NULL) {
        if (current->chunk.loaded) {
            Chunk *chunk = &current->chunk;
            int pixelX = chunk->chunkX * CHUNK_PIXEL_SIZE;
            int pixelY = chunk->chunkY * CHUNK_PIXEL_SIZE;
            
            // Frustum culling: solo dibujar chunks visibles
            if (pixelX + CHUNK_PIXEL_SIZE < camLeft || pixelX > camRight ||
                pixelY + CHUNK_PIXEL_SIZE < camTop || pixelY > camBottom) {
                current = current->next;
                continue;
            }

            // Dibujar solo tiles visibles dentro del chunk
            int startX = (int)fmaxf(0, (camLeft - pixelX) / TILE_SIZE);
            int endX = (int)fminf(CHUNK_SIZE, (camRight - pixelX) / TILE_SIZE + 1);
            int startY = (int)fmaxf(0, (camTop - pixelY) / TILE_SIZE);
            int endY = (int)fminf(CHUNK_SIZE, (camBottom - pixelY) / TILE_SIZE + 1);

            for (int y = startY; y < endY; y++) {
                for (int x = startX; x < endX; x++) {
                    int tileType = chunk->tiles[y][x];
                    DrawTexture(tiles[tileType], 
                               pixelX + x * TILE_SIZE, 
                               pixelY + y * TILE_SIZE, 
                               WHITE);
                }
            }
        }
        current = current->next;
    }
}

void GetChunkStats(ChunkManager *cm, int *loaded, int *total) {
    *loaded = cm->loadedCount;
    *total = cm->totalGenerated;
}

void UnloadChunkManager(ChunkManager *cm) {
    ChunkNode *current = cm->head;
    while (current != NULL) {
        ChunkNode *next = current->next;
        free(current);
        current = next;
    }
    cm->head = NULL;
    cm->loadedCount = 0;
    cm->totalGenerated = 0;
}