#ifndef TILES_H
#define TILES_H

#include "raylib.h"

#define TILE_COUNT 5

void LoadTiles(Texture2D tiles[TILE_COUNT]);
void UnloadTiles(Texture2D tiles[TILE_COUNT]);

#endif
