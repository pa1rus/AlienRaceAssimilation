#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include "globals.h"
#include "player.h"
#include "maps.h"

extern Camera2D gameCamera;

void InitGameCamera(void);
void UpdateGameCamera(float deltaTime);

#endif
