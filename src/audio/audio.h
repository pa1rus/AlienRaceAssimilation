#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include "globals.h"

extern Music countdownMusic;
extern Music gameMusic;
extern Sound engineSound;

void InitAudio();
void StartGameAudio();
void UpdateAudio();
void UnloadAudio();

#endif
