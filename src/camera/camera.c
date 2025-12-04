#include "camera.h"

Camera2D gameCamera = {0};

void InitGameCamera(void)
{
    gameCamera.target = (Vector2){0.0f, 0.0f};
    gameCamera.offset = (Vector2){(float)GAME_WIDTH * 0.5f, (float)GAME_HEIGHT * 0.5f};
    gameCamera.rotation = 0.0f;
    gameCamera.zoom = 1.0f;
}

static void UpdateCameraCenter(Vector2 playerPos, float delta)
{
    const float followSpeed = 1.5f;

    gameCamera.offset = (Vector2){(float)GAME_WIDTH * 0.5f, (float)GAME_HEIGHT * 0.5f};

    float t = 1.0f - powf(0.001f, followSpeed * delta);

    Vector2 diff = Vector2Subtract(playerPos, gameCamera.target);
    Vector2 step = Vector2Scale(diff, t);
    gameCamera.target = Vector2Add(gameCamera.target, step);
}

void UpdateGameCamera(float delta)
{
    Vector2 playerCenter = {
        player.rect.x + player.rect.width * 0.5f,
        player.rect.y + player.rect.height * 0.5f};
    UpdateCameraCenter(playerCenter, delta);
}

Camera2D *GetGameCamera(void)
{
    return &gameCamera;
}
