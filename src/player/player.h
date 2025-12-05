#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "maps.h"
#include "animations.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct
{
    Rectangle rect;
    Vector2 vel;
    Vector2 thrust;
    float rotationSpeed;
    double angle;
    float radius;
    AnimationID activeAnimation;
} Player;

extern Player player;

void InitPlayer();
void UpdatePlayer();
void DrawPlayer();

#endif
