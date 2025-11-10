#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

typedef struct {
    Camera2D cam;
    int radius; // radio en tiles
    float worldWidth;  // Nuevo
    float worldHeight; // Nuevo
} PlayerCamera;

void InitPlayerCamera(PlayerCamera *pcam, float x, float y, int radius);
void UpdatePlayerCamera(PlayerCamera *pcam, float x, float y);
void SetCameraWorldBounds(PlayerCamera *pcam, float worldWidth, float worldHeight); // Nuevo

#endif