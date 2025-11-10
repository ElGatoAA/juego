#include <stdlib.h>
#include <time.h>
#include <stdio.h>  // Agregar esta línea
#include "raylib.h"
#include "gen/world/noise.h"
#include "gen/world/tiles.h"
#include "gen/world/chunk.h"
#include "player/player.h"
#include "player/movement.h"
#include "camera.h"
#include "save.h"

#define TILE_SIZE 16
#define CAMERA_RADIUS 10
#define RENDER_DISTANCE 3

int main(void) {
    const int windowWidth = 512;
    const int windowHeight = 512;
    InitWindow(windowWidth, windowHeight, "Perlin World - Procedural");
    SetTargetFPS(60);

    srand(time(NULL));
    initNoise();

    Texture2D tiles[TILE_COUNT];
    LoadTiles(tiles);

    ChunkManager chunkManager;
    InitChunkManager(&chunkManager, RENDER_DISTANCE);

    Player player;
    
    // Intentar cargar mundo guardado
    if (SaveExists()) {
        printf("Cargando mundo guardado...\n");
        InitPlayer(&player, 0, 0);
        if (!LoadWorld(&chunkManager, &player)) {
            InitPlayer(&player, 256, 256);
        }
    } else {
        printf("Creando nuevo mundo...\n");
        InitPlayer(&player, 256, 256);
    }

    PlayerCamera pcam;
    InitPlayerCamera(&pcam, player.move.x, player.move.y, CAMERA_RADIUS);
    pcam.cam.offset = (Vector2){ windowWidth / 2.0f, windowHeight / 2.0f };
    pcam.cam.zoom = 1.0f;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        UpdatePlayer(&player, delta);
        UpdateChunks(&chunkManager, player.move.x, player.move.y);
        UpdatePlayerCamera(&pcam, player.move.x, player.move.y);

        if (IsKeyPressed(KEY_F5)) {
            if (SaveWorld(&chunkManager, &player)) {
                printf("¡Mundo guardado exitosamente!\n");
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            
            BeginMode2D(pcam.cam);
                DrawChunks(&chunkManager, tiles, &pcam.cam, windowWidth, windowHeight);
                DrawPlayer(&player);
            EndMode2D();
            
            int loadedChunks, totalChunks;
            GetChunkStats(&chunkManager, &loadedChunks, &totalChunks);
            
            DrawText(TextFormat("Pos: (%.0f, %.0f)", player.move.x, player.move.y), 10, 10, 20, WHITE);
            DrawText(TextFormat("Chunks cargados: %d", loadedChunks), 10, 35, 20, GREEN);
            DrawText(TextFormat("Chunks explorados: %d", totalChunks), 10, 60, 20, YELLOW);
            DrawText(TextFormat("Chunk actual: (%d, %d)", 
                     (int)(player.move.x / CHUNK_PIXEL_SIZE), 
                     (int)(player.move.y / CHUNK_PIXEL_SIZE)), 10, 85, 20, SKYBLUE);
            DrawText("F5: Guardar mundo", 10, 110, 20, ORANGE);
            DrawFPS(10, 135);
        EndDrawing();
    }

    printf("Guardando mundo antes de salir...\n");
    SaveWorld(&chunkManager, &player);

    UnloadChunkManager(&chunkManager);
    UnloadTiles(tiles);
    UnloadPlayer(&player);
    CloseWindow();
    return 0;
}