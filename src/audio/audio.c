#include "audio.h"

Music countdownMusic;
Music gameMusic;

bool countdownFinished = false;

void InitAudio() {
    InitAudioDevice();

    countdownMusic = LoadMusicStream(COUNTDOWN_MUSIC_PATH);
    SetMusicVolume(countdownMusic, 0.25f);

    gameMusic = LoadMusicStream(GAME_MUSIC_PATH);
    SetMusicVolume(gameMusic, 0.25f);

    countdownMusic.looping = false;
    gameMusic.looping = true;
}

void StartMusic() {
    countdownFinished = false;

    SeekMusicStream(countdownMusic, 0);
    PlayMusicStream(countdownMusic);
}

void UpdateAudio() {
    UpdateMusicStream(countdownMusic);
    UpdateMusicStream(gameMusic);

    if (!countdownFinished) {

        if (!IsMusicStreamPlaying(countdownMusic)) {
            countdownFinished = true;

            SeekMusicStream(gameMusic, 0);
            PlayMusicStream(gameMusic);
        }
    }
}

void UnloadAudio() {
    
    StopMusicStream(countdownMusic);
    StopMusicStream(gameMusic);

    UnloadMusicStream(countdownMusic);
    UnloadMusicStream(gameMusic);
    CloseAudioDevice();
}
