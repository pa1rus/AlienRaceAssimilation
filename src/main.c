#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "audio.h"
#include "hermes.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Platformer");

    InitGame();

    SetTargetFPS(60);

    hermesInit();

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    hermesDeinit();
    UnloadGame();
    CloseWindow();
    return 0;
}
