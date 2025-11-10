#include "animation.h"
#include <stdio.h>

void InitAnimation(Animation *anim) {
    anim->currentFrame = 0;
    anim->frameTime = 0.2f;
    anim->timer = 0.0f;

    // Cargar los 16 sprites del jugador
    for (int i = 0; i < PLAYER_FRAME_COUNT; i++) {
        char path[64];
        sprintf(path, "res/player/%d.png", i);
        anim->frames[i] = LoadTexture(path);
    }
}

void UpdateAnimation(Animation *anim, int startFrame, int endFrame, float delta) {
    anim->timer += delta;
    if (anim->timer >= anim->frameTime) {
        anim->currentFrame++;
        if (anim->currentFrame > endFrame || anim->currentFrame < startFrame)
            anim->currentFrame = startFrame;
        anim->timer = 0.0f;
    }
}

void UnloadAnimation(Animation *anim) {
    for (int i = 0; i < PLAYER_FRAME_COUNT; i++) {
        UnloadTexture(anim->frames[i]);
    }
}
