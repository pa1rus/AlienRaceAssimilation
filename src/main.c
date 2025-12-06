#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "audio.h"

int main()
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE); 
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Platformer");
    InitGame();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    UnloadGame();
    CloseWindow();
    return 0;
}
