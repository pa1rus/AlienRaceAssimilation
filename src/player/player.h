#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "maps.h"
#include "animations.h"
#include "gui.h"
#include "finish.h"
#include "game.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    int forwardKey;
    int leftKey;
    int rightKey;
} Keys;

typedef struct
{
    Rectangle rect;
    Vector2 vel;
    Vector2 thrust;
    float rotationSpeed;
    double angle;
    float radius;
    AnimationID activeAnimation;
    Keys keys;
} Player;

extern Player player;
extern Player playerTwo;

void InitPlayers();
void ResetPlayers();
void UpdatePlayers();
void DrawPlayers();
#endif
