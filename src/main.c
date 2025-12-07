#include "raylib.h"
#include "globals.h"
#include "game.h"
#include "audio.h"

#if defined(PLATFORM_WEB)
    #include "emscripten/emscripten.h"
#endif

int main()
{
    //SetConfigFlags(FLAG_FULLSCREEN_MODE); 
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Platformer");
    //hermesInit();
    //hermesGetUuid(&player.id);


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

    //hermesDeinit();
    UnloadGame();
    CloseWindow();
    return 0;
}
