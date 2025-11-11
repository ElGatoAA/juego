#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "raylib.h"
#include "gen/world/noise.h"
#include "gen/world/tiles.h"
#include "gen/world/chunk.h"
#include "player/player.h"
#include "player/movement.h"
#include "camera.h"
#include "save.h"
#include "player/inventory.h"

#define TILE_SIZE 16
#define CAMERA_RADIUS 10
#define RENDER_DISTANCE 3
#define AUTO_SAVE_INTERVAL 30.0f

int main(void) {
    int windowWidth = 1280;   // 16:9 aspect ratio
    int windowHeight = 720;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, "Perlin World - Procedural");
    SetTargetFPS(60);

    srand(time(NULL));
    
    int worldSeed;
    
    Texture2D tiles[TILE_COUNT];
    LoadTiles(tiles);

    ChunkManager chunkManager;
    InitChunkManager(&chunkManager, RENDER_DISTANCE);

    Player player;
    
    // Intentar cargar mundo guardado
    if (SaveExists()) {
        printf("Cargando mundo guardado...\n");
        InitPlayer(&player, 0, 0);
        if (LoadWorld(&chunkManager, &player, &worldSeed)) {
            setNoiseSeed(worldSeed);
            printf("Semilla del mundo restaurada: %d\n", worldSeed);
        } else {
            initNoise();
            worldSeed = getNoiseSeed();
            InitPlayer(&player, 256, 256);
            printf("Nuevo mundo creado con semilla: %d\n", worldSeed);
        }
    } else {
        printf("Creando nuevo mundo...\n");
        initNoise();
        worldSeed = getNoiseSeed();
        InitPlayer(&player, 256, 256);
        printf("Semilla del mundo: %d\n", worldSeed);
    }

    PlayerCamera pcam;
    InitPlayerCamera(&pcam, player.move.x, player.move.y, CAMERA_RADIUS);
    pcam.cam.offset = (Vector2){ windowWidth / 2.0f, windowHeight / 2.0f };
    pcam.cam.zoom = 2.0f;

    float autoSaveTimer = 0.0f;
    int showSaveMessage = 0;
    float saveMessageTimer = 0.0f;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Actualizar dimensiones de ventana si cambió
        if (IsWindowResized()) {
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();
            pcam.cam.offset = (Vector2){ windowWidth / 2.0f, windowHeight / 2.0f };
            printf("Ventana redimensionada: %dx%d\n", windowWidth, windowHeight);
        }

        // Zoom con rueda del mouse
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            pcam.cam.zoom += wheel * 0.1f;
            if (pcam.cam.zoom < 1.5f) pcam.cam.zoom = 1.5f;
            if (pcam.cam.zoom > 3.0f) pcam.cam.zoom = 3.0f;
        }

        UpdatePlayer(&player, delta);

        // Toggle inventario con E
        if (IsKeyPressed(KEY_E)) {
            ToggleInventory(&player.inventory);
        }

        // Actualizar inventario si está abierto
        if (player.inventory.isOpen) {
            UpdateInventory(&player.inventory, GetMousePosition());
        }

        UpdateChunks(&chunkManager, player.move.x, player.move.y);
        UpdatePlayerCamera(&pcam, player.move.x, player.move.y);

        // Auto-guardado
        autoSaveTimer += delta;
        if (autoSaveTimer >= AUTO_SAVE_INTERVAL) {
            if (SaveWorld(&chunkManager, &player, worldSeed)) {
                printf("Auto-guardado completado\n");
                showSaveMessage = 1;
                saveMessageTimer = 0.0f;
            }
            autoSaveTimer = 0.0f;
        }

        // Guardar manual con F5
        if (IsKeyPressed(KEY_F5)) {
            if (SaveWorld(&chunkManager, &player, worldSeed)) {
                printf("¡Mundo guardado manualmente!\n");
                showSaveMessage = 1;
                saveMessageTimer = 0.0f;
                autoSaveTimer = 0.0f;
            }
        }

        if (showSaveMessage) {
            saveMessageTimer += delta;
            if (saveMessageTimer > 2.0f) {
                showSaveMessage = 0;
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
            
            // UI adaptativa al tamaño de ventana
            DrawText(TextFormat("Pos: (%.0f, %.0f)", player.move.x, player.move.y), 10, 10, 20, WHITE);
            DrawText(TextFormat("Chunks cargados: %d", loadedChunks), 10, 35, 20, GREEN);
            DrawText(TextFormat("Chunks explorados: %d", totalChunks), 10, 60, 20, YELLOW);
            DrawText(TextFormat("Semilla: %d", worldSeed), 10, 85, 20, VIOLET);
            DrawText(TextFormat("Zoom: %.1fx", pcam.cam.zoom), 10, 110, 20, SKYBLUE);
            DrawText("F5: Guardar | Scroll: Zoom", 10, 135, 20, ORANGE);
            
            float progress = autoSaveTimer / AUTO_SAVE_INTERVAL;
            DrawRectangle(10, 165, (int)(200 * progress), 10, DARKGREEN);
            DrawRectangleLines(10, 165, 200, 10, GREEN);
            DrawText(TextFormat("Auto-save: %.0fs", AUTO_SAVE_INTERVAL - autoSaveTimer), 220, 161, 16, GREEN);
            
            if (showSaveMessage) {
                int alpha = (int)(255 * (1.0f - saveMessageTimer / 2.0f));
                int textWidth = MeasureText("MUNDO GUARDADO", 30);
                DrawText("MUNDO GUARDADO", windowWidth / 2 - textWidth / 2, windowHeight / 2 - 20, 30, 
                        (Color){0, 255, 0, alpha});
            }
            
            DrawFPS(10, windowHeight - 30);
            DrawText(TextFormat("Ventana: %dx%d", windowWidth, windowHeight), 10, windowHeight - 55, 16, GRAY);
            // Dibujar inventario encima de todo
            DrawInventory(&player.inventory, windowWidth, windowHeight);
        EndDrawing();
    }

    printf("Guardando mundo antes de salir...\n");
    SaveWorld(&chunkManager, &player, worldSeed);

    UnloadChunkManager(&chunkManager);
    UnloadTiles(tiles);
    UnloadPlayer(&player);
    CloseWindow();
    return 0;
}