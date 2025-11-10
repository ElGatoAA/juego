#include "raylib.h"
#include "noise.h"
#include "map.h"
#include <math.h>

#define TILE_SIZE 16
#define SCALE 0.02f       // Menor = continentes más grandes
#define OCTAVES 4
#define PERSISTENCE 0.5f

// Genera ruido Perlin con varias octavas (más detalle y suavidad)
static float octaveNoise(float x, float y) {
    float total = 0;
    float amplitude = 1;
    float frequency = SCALE;
    float maxValue = 0;

    for (int i = 0; i < OCTAVES; i++) {
        total += PerlinNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= PERSISTENCE;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

void GenerateMap(int map[MAP_SIZE][MAP_SIZE]) {
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            float n = octaveNoise(x, y);

            if (n < 0.25f) map[y][x] = 4;       // dark_blue (océano profundo)
            else if (n < 0.4f) map[y][x] = 3;   // light_blue (aguas bajas)
            else if (n < 0.48f) map[y][x] = 2;  // yellow (playa)
            else if (n < 0.7f) map[y][x] = 1;   // light_green (tierra)
            else map[y][x] = 0;                 // dark_green (bosque/montaña)
        }
    }
}

void DrawMap(Texture2D tiles[5], int map[MAP_SIZE][MAP_SIZE]) {
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            int t = map[y][x];
            DrawTexture(tiles[t], x * TILE_SIZE, y * TILE_SIZE, WHITE);
        }
    }
}
