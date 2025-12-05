#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "maps.h"

typedef struct
{
    Rectangle rect;
    Vector2 vel;
    bool onGround;
} Player;

extern Player player;

void InitPlayer();
void UpdatePlayer();
void DrawPlayer();

#endif
