#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <raylib.h>
#include "globals.h"

typedef enum {
    PLAYER_IDLE = 0,
    PLAYER_FLY  = 1,
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
