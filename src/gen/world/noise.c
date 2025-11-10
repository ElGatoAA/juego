#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "noise.h"

static int seed = 0;

// Inicializa la semilla aleatoria
void initNoise() {
    seed = rand(); // puede controlarse con srand(time(NULL)) desde main
}

// Función auxiliar para suavizar transiciones
static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Hash simple determinista con semilla
static float hash(int x, int y) {
    int n = x + y * 57 + seed;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589)
                    & 0x7fffffff) / 1073741824.0f);
}

float PerlinNoise(float x, float y) {
    int xi = (int)x & 255;
    int yi = (int)y & 255;
    float xf = x - (int)x;
    float yf = y - (int)y;

    float u = fade(xf);
    float v = fade(yf);

    float n00 = hash(xi, yi);
    float n01 = hash(xi, yi + 1);
    float n10 = hash(xi + 1, yi);
    float n11 = hash(xi + 1, yi + 1);

    float x1 = n00 + u * (n10 - n00);
    float x2 = n01 + u * (n11 - n01);
    float value = x1 + v * (x2 - x1);

    return (value + 1.0f) / 2.0f; // Normalizar entre 0 y 1
}
