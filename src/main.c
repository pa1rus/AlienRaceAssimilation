#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "audio.h"
#include "hermes.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Platformer");
    hermesInit();
    hermesGetUuid(&player.id);

    InitGame();

    SetTargetFPS(60);


    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    hermesDeinit();
    UnloadGame();
    CloseWindow();
    return 0;
}
