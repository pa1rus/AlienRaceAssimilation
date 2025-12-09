#ifndef GUI_H
#define GUI_H

#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "saves.h"
#include "audio.h"

typedef struct {
    char lobbyName[16];
} Lobby;

extern float timeUntilCountdown;
extern float countdownTimer;
extern bool endMenuActive;
extern int countdownIndex;
extern float fadeTimer;
extern bool countdownStarted;
extern bool countdownFinished;
extern bool movementActivated;
extern bool endMenuActive;
extern float endMenuAlpha;
extern float movementTimer;
extern float lastRunTime;
extern float bestTime;


void InitGUI();
void Drawcutscene();
void RenderMenuGUI();
void RenderLobbySelectorGUI();
void RenderLobbyCreatorGUI();
void RenderWaitingGUI();
void FormatTime(char *buffer, int size, float timeSec);
void RenderCreditsGUI();
void UpdateInGameGUI();
void DrawInGameGUI();
void DrawEndingScreen();

void StartCountdown();
void UpdateCountdown();
void DrawCountdown();

void UpdateMovementTimer();
void DrawMovementTimer();
void UnloadGUI();

#endif
