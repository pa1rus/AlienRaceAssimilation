#ifndef FINISH_H
#define FINISH_H

#include "raylib.h"
#include <raymath.h>
#include "globals.h"
#include "animations.h"
#include "maps.h"
#include "player.h"
#include "gui.h"

typedef struct {
    Rectangle rect;
    AnimationID animTopID;
    AnimationID animBottomID;
} Finish;

extern Finish finish;
extern bool playerFinished;

void InitFinish();
void UpdateFinish();
void DrawFinishTop();
void DrawFinishBottom();

#endif
