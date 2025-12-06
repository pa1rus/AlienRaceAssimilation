#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include "globals.h"

extern Music countdownMusic;
extern Music gameMusic;

void InitAudio();
void StartMusic();
void UnloadAudio();

#endif
