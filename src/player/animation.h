#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"

#define PLAYER_FRAME_COUNT 16

typedef struct {
    Texture2D frames[PLAYER_FRAME_COUNT];
    int currentFrame;
    float frameTime;
    float timer;
} Animation;

void InitAnimation(Animation *anim);
void UpdateAnimation(Animation *anim, int startFrame, int endFrame, float delta);
void UnloadAnimation(Animation *anim);

#endif
