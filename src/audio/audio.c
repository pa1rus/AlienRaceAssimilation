#include "audio.h"

Music countdownMusic;
Music gameMusic;
Music menuMusic;
Sound engineSound;

bool menuMusicStarted = false;
float engineDelayTimer = 0.0f;

void InitAudio()
{
    InitAudioDevice();

    countdownMusic = LoadMusicStream(COUNTDOWN_MUSIC_PATH);
    SetMusicVolume(countdownMusic, 0.25f);
    countdownMusic.looping = false;

    gameMusic = LoadMusicStream(GAME_MUSIC_PATH);
    SetMusicVolume(gameMusic, 0.25f);
    gameMusic.looping = true;

    menuMusic = LoadMusicStream(MENU_MUSIC_PATH);
    SetMusicVolume(menuMusic, 0.25f);
    menuMusic.looping = true;

    engineSound = LoadSound(ENIGINE_SOUND_PATH);
    SetSoundVolume(engineSound, 0.35f);
}

void StartMenuAudio()
{
    if (!menuMusicStarted)
    {
        menuMusicStarted = true;
        SeekMusicStream(menuMusic, 0);
        PlayMusicStream(menuMusic);
        HideCursor();
    }
}

void StartGameAudio()
{
    engineDelayTimer = 0.0f;
    PlaySound(engineSound);
}

void UpdateAudio()
{
    if (gameState != GAME)
    {
        UpdateMusicStream(menuMusic);
        return;
    }

    float dt = GetFrameTime();
    UpdateMusicStream(countdownMusic);
    UpdateMusicStream(gameMusic);

    engineDelayTimer += dt;

    if (!countdownStarted && engineDelayTimer >= timeUntilCountdown)
    {
        countdownStarted = true;
        SeekMusicStream(countdownMusic, 0);
        PlayMusicStream(countdownMusic);
    }

    if (countdownFinished && !IsMusicStreamPlaying(countdownMusic) && !IsMusicStreamPlaying(gameMusic))
    {
        SeekMusicStream(gameMusic, 0);
        PlayMusicStream(gameMusic);
    }
}


void UnloadAudio()
{
    StopMusicStream(countdownMusic);
    StopMusicStream(gameMusic);
    StopMusicStream(menuMusic);

    UnloadMusicStream(countdownMusic);
    UnloadMusicStream(gameMusic);
    UnloadMusicStream(menuMusic);

    UnloadSound(engineSound);
    CloseAudioDevice();
}
