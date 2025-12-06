#ifndef FINISH_H
#define FINISH_H

#include "raylib.h"
#include "globals.h"
#include "animations.h"
#include "maps.h"
#include "player.h"

typedef struct {
    Rectangle rect;
    AnimationID animTopID;
    AnimationID animBottomID;
} Finish;

extern Finish finish;

void InitFinish();
void UpdateFinish();
void DrawFinishTop();
void DrawFinishBottom();

#endif
