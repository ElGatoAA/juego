#include "movement.h"
#include "raylib.h"
#include <math.h>

void InitMovement(Movement *move, float startX, float startY) {
    move->x = startX;
    move->y = startY;
    move->dir = DIR_DOWN;
    move->moving = 0;
}

void UpdateMovement(Movement *move, float speed) {
    move->moving = 0;

    float dx = 0;
    float dy = 0;

    // Movimiento horizontal
    if (IsKeyDown(KEY_A)) dx -= 1.0f;
    if (IsKeyDown(KEY_D)) dx += 1.0f;

    // Movimiento vertical
    if (IsKeyDown(KEY_W)) dy -= 1.0f;
    if (IsKeyDown(KEY_S)) dy += 1.0f;

    // Normalizar para velocidad uniforme en diagonal
    if (dx != 0 || dy != 0) {
        float length = sqrtf(dx*dx + dy*dy);
        dx /= length;
        dy /= length;

        move->x += dx * speed;
        move->y += dy * speed;
        move->moving = 1;

        // Actualizar dirección principal (para la animación)
        if (fabs(dx) > fabs(dy)) {
            move->dir = dx > 0 ? DIR_RIGHT : DIR_LEFT;
        } else {
            move->dir = dy > 0 ? DIR_DOWN : DIR_UP;
        }
    }
}

// ... tu código existente ...

void ClampMovementToWorld(Movement *move, float worldWidth, float worldHeight, float playerSize) {
    if (move->x < 0.0f) 
        move->x = 0.0f;
    if (move->x > worldWidth - playerSize) 
        move->x = worldWidth - playerSize;
    if (move->y < 0.0f) 
        move->y = 0.0f;
    if (move->y > worldHeight - playerSize) 
        move->y = worldHeight - playerSize;
}