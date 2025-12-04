#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>

#define TILESET_PATH "./assets/maps/tiles.png"
#define MAP_JSON_PATH "./assets/maps/maps.json"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int FPS;

extern RenderTexture2D target;
extern int scaledW;
extern int scaledH;

#endif
