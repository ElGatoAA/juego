#ifndef MOVEMENT_H
#define MOVEMENT_H

typedef enum {
    DIR_DOWN,
    DIR_UP,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

typedef struct {
    float x, y;
    Direction dir;
    int moving;
} Movement;

void InitMovement(Movement *move, float startX, float startY);
void UpdateMovement(Movement *move, float speed);
// Restringe el movimiento dentro de los límites del mundo
void ClampMovementToWorld(Movement *move, float worldWidth, float worldHeight, float playerSize);

#endif