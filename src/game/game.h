#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "globals.h"
#include "maps.h"
#include "player.h"
#include "camera.h"
#include "animations.h"

void InitGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void UnloadGame();

extern RenderTexture2D target;
extern int scaledW;
extern int scaledH;

#endif
