#include <raylib.h>
#include "globals.h"
#include "game.h"
#include "audio.h"

RenderTexture2D target;
int scaledW, scaledH;

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Platformer");

    target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitAudio();

    InitGame();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        scaledW = GetScreenWidth();
        scaledH = GetScreenHeight();

        UpdateDrawFrame();
    }

    UnloadGame();
    CloseWindow();
    return 0;
}
