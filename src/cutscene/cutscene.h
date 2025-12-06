#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "raylib.h"
#include "globals.h"
#include "game.h"

typedef struct {
    Texture2D bg;
    Texture2D text;
} CutsceneSlide;

typedef enum {
    CS_FADE_IN_BG,
    CS_FADE_IN_TEXT,
    CS_FADE_IN_TEXT_2,
    CS_HOLD,
    CS_FADE_OUT,
    CS_DONE
} CutState;

typedef struct {
    char *text;
    Vector2 position;
} Text2;

void InitCutscene();
void UpdateCutscene();
void DrawCutscene();
void UnloadCutscene();

#endif