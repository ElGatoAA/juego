#ifndef NOISE_H
#define NOISE_H

void initNoise();
float PerlinNoise(float x, float y);

// Nuevas funciones
void setNoiseSeed(int newSeed);
int getNoiseSeed(void);

#endif