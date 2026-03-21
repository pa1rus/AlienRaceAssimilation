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
#include "background.h"

typedef enum
{
    CUTSCENE = 0,
    //kurwa mac nie, nie bede zmienial tych nazw to jest za duzo pierdolenia sie ja chce do domu beda te stare i te nowe
    MENU,
    // single albo duo
    MODE_MENU,
    GAME,
    CREDITS
} GameState;

extern int gameState;
extern bool gameStarted;
extern bool pause;
extern bool multi;

extern RenderTexture2D target;
extern int scaledW;
extern int scaledH;

void InitGame();
void StartGame();
void PrepareGame();
void UpdateGame();
void DrawGame();
void UpdateDrawFrame();
void UnloadGame();

#endif
