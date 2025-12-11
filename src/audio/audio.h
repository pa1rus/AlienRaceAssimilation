#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "gui.h"

extern Music countdownMusic;
extern Music gameMusic;
extern Music menuMusic;
extern Sound engineSound;
extern float volume;

void InitAudio();
void StartGameAudio();
void StartMenuAudio();
void UpdateAudio();
void UnloadAudio();

#endif
