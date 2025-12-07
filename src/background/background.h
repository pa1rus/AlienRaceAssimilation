#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"
#include "globals.h"
#include <math.h>

typedef struct {
    Texture2D texture;
    float scrollSpeedX;
    float scrollSpeedY;
    Vector2 offset;
} ParallaxLayer;

void InitBackground();
void UpdateBackground(Vector2 cameraPos);
void DrawBackground();
void UnloadBackground();

#endif
