#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include "globals.h"
#include "game.h"

extern Music countdownMusic;
extern Music gameMusic;
extern Music menuMusic;
extern Sound engineSound;

void InitAudio();
void StartGameAudio();
void StartMenuAudio();
void UpdateAudio();
void UnloadAudio();

#endif
