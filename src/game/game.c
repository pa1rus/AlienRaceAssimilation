#include "game.h"
#include "hermes.h"
#include <uuid/uuid.h>

RenderTexture2D target;
int scaledW, scaledH;

int gameState = MENU;
bool gameStarted = false;

void InitGame()
{

    target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitGUI();
    InitMaps();
    InitPlayer();
    InitGameCamera();
    InitAnimations();
    InitFinish();
    InitAudio();
}

void UpdateGame()
{
    switch (gameState)
    {
    case MENU:
        UpdateAudio();
        StartMenuAudio();

        break;
    case LOBBY_SELECTOR:
        UpdateAudio();

        break;
    case GAME:

        if (!gameStarted)
        {
            StartGameAudio();
            StartCountdown();
            gameStarted = true;
        }
        UpdatePlayer();
        UpdateFinish();
        UpdateGameCamera();
        UpdateAnimations();
        UpdateInGameGUI();
        UpdateAudio();
        break;
    case CREDITS:
        UpdateAudio();
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
    case MENU:
        RenderMenuGUI();

        break;
    case LOBBY_SELECTOR:
        RenderLobbySelectorGUI();

        break;
    case LOBBY_CREATOR:
        RenderLobbyCreatorGUI();
        break;
    case WAITING:
        RenderWaitingGUI();

        break;
    case GAME:
        BeginMode2D(gameCamera);
        DrawCurrentMap();
        DrawFinishBottom();
        DrawPlayer();
        DrawFinishTop();
        EndMode2D();
        DrawInGameGUI();

        break;
    case CREDITS:
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
    UnloadRenderTexture(target);
}
