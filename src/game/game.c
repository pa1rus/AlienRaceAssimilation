#include "game.h"

RenderTexture2D target;
int scaledW, scaledH;

void InitGame()
{
    target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitMaps();
    InitPlayer();
    InitGameCamera();
    InitAnimations();
}

void UpdateGame()
{
    float deltaTime = GetFrameTime();
    UpdatePlayer();
    UpdateGameCamera(deltaTime);
    UpdateAnimations();
}

void DrawGame()
{
    scaledW = GetScreenWidth();
    scaledH = GetScreenHeight();

    BeginTextureMode(target);
    Color c = (Color){32, 57, 79, 255};
    ClearBackground(c);
    BeginMode2D(gameCamera);
    DrawCurrentMap();
    DrawPlayer();
    EndMode2D();
    EndTextureMode();

    BeginDrawing();
    Rectangle src = {0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height};
    Rectangle dst = {0.0f, 0.0f, (float)scaledW, (float)scaledH};
    DrawTexturePro(target.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
}

void UpdateDrawFrame()
{
    UpdateGame();
    DrawGame();
}

void UnloadGame()
{
    if (gameMapData.tilesetTexture.id != 0)
        UnloadTexture(gameMapData.tilesetTexture);
}
