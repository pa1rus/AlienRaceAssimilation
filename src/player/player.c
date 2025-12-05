#include "player.h"
#include "maps.h"

#define MAX_SPEED 18.0f

Player player = {0};

const Vector2 playerSpawnPoints[] = {
    (Vector2){3.0f, 0.0f},
    (Vector2){5.0f, 1.0f},
};

void InitPlayer()
{
    float tileSize = (float)gameMapData.tileSize;
    float tileScale = (float)gameMapData.tileScale;
    player.rect = (Rectangle){
        playerSpawnPoints[gameMapData.currentMapIndex].x * tileSize * tileScale,
        playerSpawnPoints[gameMapData.currentMapIndex].y * tileSize * tileScale,
        tileSize * tileScale,
        tileSize * tileScale};
    player.vel = (Vector2){0.0f, 0.0f};
    player.thrust = (Vector2){0.0f, -0.25f};
    player.angle = 0.0;
    player.rotationSpeed = 180.0f;
}

Vector2 rotate(Vector2 coordinates, double angle, Vector2 anchor)
{
    coordinates = Vector2Subtract(coordinates, anchor);

    double radians = angle * (M_PI / 180.0);
    double rotation_matrix[2][2] = {
        {cos(radians), sin(radians) * -1},
        {sin(radians), cos(radians)},
    };

    double x = (rotation_matrix[0][0] * coordinates.x) + (rotation_matrix[0][1] * coordinates.y);
    double y = (rotation_matrix[1][0] * coordinates.x) + (rotation_matrix[1][1] * coordinates.y);
    return Vector2Add((Vector2){x, y}, anchor);
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
                Tile *tile = &layer->tiles[row][col];
                if (!tile->indices || tile->indiceCount <= 0)
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
    if (IsKeyDown(KEY_W))
    {
        player.vel = Vector2Add(player.vel, rotate(player.thrust, player.angle, (Vector2){0, 0}));
    }
    if (IsKeyDown(KEY_D))
    {
        player.angle += GetFrameTime() * player.rotationSpeed;
        player.angle = fmod(player.angle, 360.0);
        if (player.angle < 0)
            player.angle += 360.0;
    }
    if (IsKeyDown(KEY_A))
    {
        player.angle -= GetFrameTime() * player.rotationSpeed;
        player.angle = fmod(player.angle, 360.0);
        if (player.angle < 0)
            player.angle += 360.0;
    }

    float speed = Vector2Length(player.vel);
    if (speed > MAX_SPEED) {
        player.vel = Vector2Scale(player.vel, MAX_SPEED / speed);
    }

    Rectangle newRect = player.rect;
    newRect.x += player.vel.x;
    if (!CheckCollisionWithTiles(newRect)) {
        player.rect.x = newRect.x;
    } else {
        player.vel.x = -player.vel.x * 0.5f;
    }

    newRect = player.rect;
    newRect.y += player.vel.y;
    if (!CheckCollisionWithTiles(newRect)) {
        player.rect.y = newRect.y;
    } else {
        player.vel.y = -player.vel.y * 0.5f;
    }
}

void DrawPlayer()
{
    Vector2 origin = {player.rect.width / 2.0f, player.rect.height / 2.0f};
    DrawRectanglePro(player.rect, origin, (float)player.angle, WHITE);
}
