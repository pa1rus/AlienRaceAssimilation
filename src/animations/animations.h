#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "raylib.h"
#include "globals.h"

typedef enum {
    PLAYER_IDLE = 0,
    PLAYER_FLY  = 1,
    FINISH_IDLE_TOP = 2,
    FINISH_IDLE_BOTTOM = 3,
    FINISH_OPEN_TOP = 4,
    FINISH_OPEN_BOTTOM = 5,
    FINISH_TRANSITION_TOP = 6,
    FINISH_TRANSITION_BOTTOM = 7,
    ANIMATIONS_COUNT
} AnimationID;

typedef struct {
    Texture2D texture;
    Rectangle frameRect;
    int frameCount;
    int currentFrame;
    float fps;
    float frameTimer;
} Animation;

extern Animation animations[ANIMATIONS_COUNT];

void InitAnimations(void);
void UpdateAnimations(void);
void DrawAnimationAt(AnimationID id, Rectangle dest, float angle, float scale);
void UnloadAnimations(void);

#endif
