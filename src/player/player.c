#include "player.h"
#include "maps.h"

#define GRAVITY 1.5f
#define MOVE_SPEED 6.0f
#define MAX_FALL_SPEED 12.0f
#define JUMP_FORCE -18.0f

Player player = {0};

void InitPlayer()
{
    float tileSize = (float)gameMapData.tileSize;
    float tileScale = (float)gameMapData.tileScale;
    player.rect = (Rectangle){
        3.0f * tileSize * tileScale,
        1.0f * tileSize * tileScale,
        16.0f * tileScale,
        32.0f * tileScale};
    player.vel = (Vector2){0.0f, 0.0f};
    player.onGround = false;
}

bool CheckCollisionWithTiles(Rectangle playerRect)
{
    Map *currentMap = &gameMapData.activeMap;
    if (!currentMap || currentMap->layerCount <= 0)
        return false;

    float tileSize = (float)gameMapData.tileSize;
    float tileScale = (float)gameMapData.tileScale;
    float scaledTileSize = tileSize * tileScale;

    for (int i = 0; i < currentMap->layerCount; i++)
    {
        if (!currentMap->layers[i].collisions)
            continue;

        Layer *layer = &currentMap->layers[i];
        for (int row = 0; row < currentMap->gridRows; row++)
        {
            for (int col = 0; col < currentMap->gridCols; col++)
            {
                int tileIndex = layer->tiles[row][col].index;
                if (tileIndex < 0)
                    continue;

                Rectangle tileRect = {
                    (float)col * scaledTileSize,
                    (float)row * scaledTileSize,
                    scaledTileSize,
                    scaledTileSize};

                if (CheckCollisionRecs(playerRect, tileRect))
                    return true;
            }
        }
    }
    return false;
}

void UpdatePlayer()
{
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        player.vel.x = -MOVE_SPEED;
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        player.vel.x = MOVE_SPEED;
    else
        player.vel.x = 0.0f;

    if (IsKeyPressed(KEY_SPACE) && player.onGround)
        player.vel.y = JUMP_FORCE;

    player.vel.y += GRAVITY;
    if (player.vel.y > MAX_FALL_SPEED)
        player.vel.y = MAX_FALL_SPEED;

    Rectangle newRect = player.rect;
    newRect.x += player.vel.x;
    if (!CheckCollisionWithTiles(newRect))
        player.rect.x = newRect.x;

    player.onGround = false;
    newRect = player.rect;
    newRect.y += player.vel.y;
    if (CheckCollisionWithTiles(newRect))
    {
        player.vel.y = 0.0f;
        player.onGround = true;
    }
    else
    {
        player.rect.y = newRect.y;
    }
}

void DrawPlayer()
{
    DrawRectangleRec(player.rect, WHITE);
}
