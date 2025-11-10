#ifndef MAP_H
#define MAP_H

#include "raylib.h"   // <- necesario para Texture2D

#define MAP_SIZE 64

void GenerateMap(int map[MAP_SIZE][MAP_SIZE]);
void DrawMap(Texture2D tiles[5], int map[MAP_SIZE][MAP_SIZE]);

#endif
