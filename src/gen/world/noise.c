#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "noise.h"

static int seed = 0;

// Tabla de permutación para Perlin Noise
static int p[512];

void initNoise() {
    seed = rand();
    
    // Inicializar tabla de permutación
    int permutation[256];
    for (int i = 0; i < 256; i++) {
        permutation[i] = i;
    }
    
    // Mezclar usando el seed
    srand(seed);
    for (int i = 255; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = temp;
    }
    
    // Duplicar para evitar desbordamiento
    for (int i = 0; i < 512; i++) {
        p[i] = permutation[i % 256];
    }
}

void setNoiseSeed(int newSeed) {
    seed = newSeed;
    
    // Reinicializar tabla de permutación con nuevo seed
    int permutation[256];
    for (int i = 0; i < 256; i++) {
        permutation[i] = i;
    }
    
    srand(seed);
    for (int i = 255; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = temp;
    }
    
    for (int i = 0; i < 512; i++) {
        p[i] = permutation[i % 256];
    }
}

int getNoiseSeed(void) {
    return seed;
}

// Función de fade mejorada (smoothstep cúbico)
static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Interpolación lineal
static float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

// Función de gradiente
static float grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise(float x, float y) {
    // Encontrar las coordenadas de la celda
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    
    // Posición relativa dentro de la celda
    x -= floor(x);
    y -= floor(y);
    
    // Calcular curvas de fade
    float u = fade(x);
    float v = fade(y);
    
    // Hash de las 4 esquinas
    int aa = p[p[X] + Y];
    int ab = p[p[X] + Y + 1];
    int ba = p[p[X + 1] + Y];
    int bb = p[p[X + 1] + Y + 1];
    
    // Interpolar entre los gradientes
    float x1 = lerp(u, grad(aa, x, y), grad(ba, x - 1, y));
    float x2 = lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1));
    
    return lerp(v, x1, x2);
}