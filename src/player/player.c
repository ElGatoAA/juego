#include "player.h"

void InitPlayer(Player *player, float x, float y) {
    InitMovement(&player->move, x, y);
    InitAnimation(&player->anim);
}

void UpdatePlayer(Player *player, float delta) {
    UpdateMovement(&player->move, 1.5f);

    int start, end;

    switch (player->move.dir) {
        case DIR_DOWN:  start = player->move.moving ? 2 : 0;  end = player->move.moving ? 3 : 1; break;
        case DIR_UP:    start = player->move.moving ? 6 : 4;  end = player->move.moving ? 7 : 5; break;
        case DIR_LEFT:  start = player->move.moving ? 10 : 8; end = player->move.moving ? 11 : 9; break;
        case DIR_RIGHT: start = player->move.moving ? 14 : 12;end = player->move.moving ? 15 : 13; break;
    }

    UpdateAnimation(&player->anim, start, end, delta);
}

void DrawPlayer(Player *player) {
    Texture2D frame = player->anim.frames[player->anim.currentFrame];
    DrawTexture(frame, player->move.x, player->move.y, WHITE);
}

void UnloadPlayer(Player *player) {
    UnloadAnimation(&player->anim);
}
