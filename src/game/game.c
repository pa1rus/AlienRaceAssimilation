#include "game.h"

RenderTexture2D target;
int scaledW, scaledH;

int gameState = CUTSCENE;
bool gameStarted = false;
bool menuShowed = false;

const int BLACK_ALPHA = 50;

void InitGame()
{

    target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitGUI();
    InitMaps();
    InitPlayer();
    InitGameCamera();
    InitBackground();
    InitAnimations();
    InitCutscene();
    InitFinish();
    InitAudio();
}

void UpdateGame()
{
    switch (gameState)
    {
    case CUTSCENE:
        UpdateCutscene();
        StartMenuAudio();
        UpdateAudio();

        break;
    case MENU:
        UpdateAudio();
        UpdateBackgroundAuto();
        if (!menuShowed)
        {
            ShowCursor();
            menuShowed = true;
        }

        break;
    case GAME:

        if (!gameStarted)
        {
            StartGameAudio();
            StartCountdown();
            HideCursor();
            gameStarted = true;
        }
        UpdateBackground(gameCamera.target);
        UpdatePlayer();
        UpdateFinish();
        UpdateGameCamera();
        UpdateAnimations();
        UpdateInGameGUI();
        UpdateAudio();
        break;
    case CREDITS:
        UpdateAudio();
        UpdateBackgroundAuto();
        break;
    }
}

void DrawGame()
{
    scaledW = GetScreenWidth();
    scaledH = GetScreenHeight();

    BeginTextureMode(target);
    Color c = (Color){32, 57, 79, 255};
    ClearBackground(c);

    switch (gameState)
    {
    case CUTSCENE:
        DrawCutscene();
        break;
    case MENU:
        DrawBackground();
        DrawRectangleRec((Rectangle){0, 0, GAME_WIDTH, GAME_HEIGHT}, (Color) {0, 0, 0, BLACK_ALPHA});
        RenderMenuGUI();
        break;

    case GAME:
        DrawBackground();
        DrawRectangleRec((Rectangle){0, 0, GAME_WIDTH, GAME_HEIGHT}, (Color) {0, 0, 0, BLACK_ALPHA});
        BeginMode2D(gameCamera);
        DrawCurrentMap();
        DrawFinishBottom();
        DrawPlayer();
        DrawFinishTop();
        EndMode2D();
        DrawInGameGUI();
        DrawEndingScreen();


        break;
    case CREDITS:
        DrawBackground();
        DrawRectangleRec((Rectangle){0, 0, GAME_WIDTH, GAME_HEIGHT}, (Color) {0, 0, 0, BLACK_ALPHA});
        RenderCreditsGUI();
        break;
    }

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

    UnloadAudio();
    UnloadAnimations();
    UnloadCutscene();
    UnloadRenderTexture(target);
    UnloadGUI();
}
