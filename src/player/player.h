#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "movement.h"
#include "animation.h"

typedef struct {
    Movement move;
    Animation anim;
} Player;

void InitPlayer(Player *player, float x, float y);
void UpdatePlayer(Player *player, float delta);
void DrawPlayer(Player *player);
void UnloadPlayer(Player *player);

#endif
