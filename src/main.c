#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "audio.h"

#if defined(PLATFORM_WEB)
#include "emscripten/emscripten.h"
#endif

int main()
{
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Alien Race: Assimilation");
    SetExitKey(KEY_NULL);

    #if defined(PLATFORM_WEB)
    #else

    #ifndef __APPLE__


        SetWindowState(FLAG_FULLSCREEN_MODE);
    #endif // #ifndef __APPLE__
        
    #endif

    InitGame();

    #if defined(PLATFORM_WEB)

        emscripten_set_main_loop(UpdateDrawFrame, 60, 1);

    #else

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    UnloadGame();
    CloseWindow();
    return 0;
}
