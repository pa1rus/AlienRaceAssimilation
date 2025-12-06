#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "globals.h"
#include "gui.h"
#include "maps.h"
#include "player.h"
#include "camera.h"
#include "animations.h"
#include "audio.h"
#include "finish.h"
#include "cutscene.h"

typedef enum
{
    CUTSCENE = 0,
    MENU,
    LOBBY_SELECTOR,
    LOBBY_CREATOR,
    WAITING,
    GAME,
    CREDITS
} GameState;

extern int gameState;

extern RenderTexture2D target;
extern int scaledW;
extern int scaledH;

void InitGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void UnloadGame();

#endif
