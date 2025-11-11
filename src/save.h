#ifndef SAVE_H
#define SAVE_H

#include "raylib.h"
#include "gen/world/chunk.h"
#include "player/player.h"

#define SAVE_FILE "world_save.dat"

// Guardar el mundo completo (ahora también guarda la semilla)
int SaveWorld(ChunkManager *cm, Player *player, int seed);

// Cargar el mundo completo (ahora también carga la semilla)
int LoadWorld(ChunkManager *cm, Player *player, int *seed);

int SaveExists(void);

#endif