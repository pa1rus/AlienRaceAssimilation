#include "audio.h"

Music countdownMusic;
Music gameMusic;
Music menuMusic;

Sound engineSound;

bool countdownStarted = false;
bool countdownFinished = false;
bool menuMusicStarted = false;

float engineDelayTimer = 0.0f;
float timeUntillcountdown = 2.5f;

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
    }
}

void StartGameAudio()
{
    countdownStarted = false;
    countdownFinished = false;
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
    else
    {

        float dt = GetFrameTime();

        UpdateMusicStream(countdownMusic);
        UpdateMusicStream(gameMusic);

        if (!countdownStarted)
        {
            engineDelayTimer += dt;

            if (engineDelayTimer >= timeUntillcountdown)
            {
                SeekMusicStream(countdownMusic, 0);
                PlayMusicStream(countdownMusic);

                countdownStarted = true;
            }
        }
        else if (!countdownFinished)
        {
            if (!IsMusicStreamPlaying(countdownMusic))
            {
                countdownFinished = true;

                SeekMusicStream(gameMusic, 0);
                PlayMusicStream(gameMusic);
            }
        }
    }
}

void UnloadAudio()
{
    StopMusicStream(countdownMusic);
    StopMusicStream(gameMusic);

    UnloadMusicStream(countdownMusic);
    UnloadMusicStream(gameMusic);

    UnloadSound(engineSound);

    CloseAudioDevice();
}
