#include "tiles.h"

void LoadTiles(Texture2D tiles[TILE_COUNT]) {
    tiles[0] = LoadTexture("res/tiles/0.png"); // dark_green
    tiles[1] = LoadTexture("res/tiles/1.png"); // light_green
    tiles[2] = LoadTexture("res/tiles/2.png"); // yellow
    tiles[3] = LoadTexture("res/tiles/3.png"); // light_blue
    tiles[4] = LoadTexture("res/tiles/4.png"); // dark_blue
}

void UnloadTiles(Texture2D tiles[TILE_COUNT]) {
    for (int i = 0; i < TILE_COUNT; i++) {
        UnloadTexture(tiles[i]);
    }
}
