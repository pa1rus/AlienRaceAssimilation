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
void RenderInGameGUI();

void StartCountdown();
void UpdateCountdown();
void DrawCountdown();

#endif
