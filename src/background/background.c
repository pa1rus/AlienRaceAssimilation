#include "background.h"

#define LAYER_COUNT 3

ParallaxLayer layers[LAYER_COUNT];

void InitBackground()
{
    layers[0].texture = LoadTexture(BG_1_PATH);
    layers[0].scrollSpeedX = 0.2f;
    layers[0].scrollSpeedY = 0.1f;
    layers[0].offset = (Vector2){0,0};

    layers[1].texture = LoadTexture(BG_2_PATH);
    layers[1].scrollSpeedX = 0.6f;
    layers[1].scrollSpeedY = 0.4f;
    layers[1].offset = (Vector2){0,0};

    layers[2].texture = LoadTexture(BG_3_PATH);
    layers[2].scrollSpeedX = 1.0f;
    layers[2].scrollSpeedY = 0.8f;
    layers[2].offset = (Vector2){0,0};
}

void UpdateBackground(Vector2 cameraPos)
{
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        layers[i].offset.x = fmodf(cameraPos.x * layers[i].scrollSpeedX, layers[i].texture.width);
        layers[i].offset.y = fmodf(cameraPos.y * layers[i].scrollSpeedY, layers[i].texture.height);
        if (layers[i].offset.x < 0) layers[i].offset.x += layers[i].texture.width;
        if (layers[i].offset.y < 0) layers[i].offset.y += layers[i].texture.height;
    }
}

void UpdateBackgroundAuto()
{
    float dt = GetFrameTime();
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        layers[i].offset.x -= layers[i].scrollSpeedX * 30.0f * dt;
        layers[i].offset.y += layers[i].scrollSpeedY * 30.0f * dt;

        layers[i].offset.x = fmodf(layers[i].offset.x, layers[i].texture.width);
        layers[i].offset.y = fmodf(layers[i].offset.y, layers[i].texture.height);
    }
}


void DrawBackground()
{
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        Texture2D tex = layers[i].texture;
        Vector2 off = layers[i].offset;

        int tilesX = (GAME_WIDTH / tex.width) + 3;
        int tilesY = (GAME_HEIGHT / tex.height) + 3;

        for (int y = -1; y < tilesY - 1; y++)
        {
            for (int x = -1; x < tilesX - 1; x++)
            {
                DrawTexture(tex,
                            x * tex.width - (int)off.x,
                            y * tex.height - (int)off.y,
                            WHITE);
            }
        }
    }
}


void UnloadBackground()
{
    for (int i = 0; i < LAYER_COUNT; i++)
    {
        UnloadTexture(layers[i].texture);
    }
}
