#ifndef GUI_H
#define GUI_H

#include "raylib.h"
#include "globals.h"
#include "game.h"

extern float timeUntilCountdown;
extern bool countdownStarted;
extern bool countdownFinished;
extern bool movementActivated;

void InitGUI();
void RenderMenuGUI();
void RenderLobbySelectorGUI();
void RenderCreditsGUI();
void UpdateInGameGUI();
void DrawInGameGUI();

void StartCountdown();
void UpdateCountdown();
void DrawCountdown();

void UpdateMovementTimer();
void DrawMovementTimer();

#endif
