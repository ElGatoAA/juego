#ifndef CHUNK_H
#define CHUNK_H

#include "raylib.h"

#define CHUNK_SIZE 32
#define TILE_SIZE 16
#define CHUNK_PIXEL_SIZE (CHUNK_SIZE * TILE_SIZE)

typedef struct {
    int chunkX, chunkY;
    int tiles[CHUNK_SIZE][CHUNK_SIZE];
    int loaded;
    int generated;
} Chunk;

typedef struct ChunkNode {
    Chunk chunk;
    struct ChunkNode *next;
} ChunkNode;

typedef struct {
    ChunkNode *head;
    int loadedCount;
    int totalGenerated;
    int renderDistance;
} ChunkManager;

void InitChunkManager(ChunkManager *cm, int renderDistance);
void UpdateChunks(ChunkManager *cm, float playerX, float playerY);
void DrawChunks(ChunkManager *cm, Texture2D tiles[5], Camera2D *camera, int screenWidth, int screenHeight);
void UnloadChunkManager(ChunkManager *cm);
void GetChunkStats(ChunkManager *cm, int *loaded, int *total);

#endif