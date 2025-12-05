#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "globals.h"
#include "gui.h"
#include "maps.h"
#include "player.h"
#include "camera.h"
#include "animations.h"

typedef enum
{
    MENU = 0,
    LOBBY_SELECTOR,
    GAME,
    EXIT
} GameState;

extern RenderTexture2D target;
extern int scaledW;
extern int scaledH;

void InitGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void UnloadGame();

#endif
