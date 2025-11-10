#include "camera.h"

void InitPlayerCamera(PlayerCamera *pcam, float x, float y, int radius) {
    pcam->radius = radius;
    pcam->cam.target = (Vector2){ x, y };
    pcam->cam.offset = (Vector2){ 0, 0 }; // se ajustará en main
    pcam->cam.rotation = 0.0f;
    pcam->cam.zoom = 1.0f; // se ajustará en main
    pcam->worldWidth = 0;
    pcam->worldHeight = 0;
}

void UpdatePlayerCamera(PlayerCamera *pcam, float x, float y) {
    // Calcular los límites de la cámara basados en el tamaño de la ventana
    float halfScreenWidth = pcam->cam.offset.x / pcam->cam.zoom;
    float halfScreenHeight = pcam->cam.offset.y / pcam->cam.zoom;
    
    // Limitar el target de la cámara para que no muestre fuera del mundo
    float targetX = x;
    float targetY = y;
    
    if (pcam->worldWidth > 0 && pcam->worldHeight > 0) {
        // Límites izquierdo/derecho
        if (targetX - halfScreenWidth < 0) 
            targetX = halfScreenWidth;
        if (targetX + halfScreenWidth > pcam->worldWidth) 
            targetX = pcam->worldWidth - halfScreenWidth;
        
        // Límites arriba/abajo
        if (targetY - halfScreenHeight < 0) 
            targetY = halfScreenHeight;
        if (targetY + halfScreenHeight > pcam->worldHeight) 
            targetY = pcam->worldHeight - halfScreenHeight;
    }
    
    pcam->cam.target = (Vector2){ targetX, targetY };
}

void SetCameraWorldBounds(PlayerCamera *pcam, float worldWidth, float worldHeight) {
    pcam->worldWidth = worldWidth;
    pcam->worldHeight = worldHeight;
}