#include "audio.h"

Music countdownMusic;
Music gameMusic;

void InitAudio(){

    InitAudioDevice();

    countdownMusic = LoadMusicStream(COUNTDOWN_MUSIC_PATH);
    SetMusicVolume(countdownMusic, 0.5f);

    countdownMusic = LoadMusicStream(GAME_MUSIC_PATH);
    SetMusicVolume(countdownMusic, 0.5f);
};  

void StartMusic()
{

    SeekMusicStream(countdownMusic, 0);
    PlayMusicStream(countdownMusic);
}

void UnloadAudio()
{
    UnloadMusicStream(countdownMusic);
    UnloadMusicStream(gameMusic);
    CloseAudioDevice();
}