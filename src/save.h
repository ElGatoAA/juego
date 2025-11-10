#ifndef SAVE_H
#define SAVE_H

#include "raylib.h"
#include "gen/world/chunk.h"
#include "player/player.h"

#define SAVE_FILE "world_save.dat"

// Guardar el mundo completo
int SaveWorld(ChunkManager *cm, Player *player);

// Cargar el mundo completo
int LoadWorld(ChunkManager *cm, Player *player);

// Verificar si existe un guardado
int SaveExists(void);

#endif