#include "player.h"

#define MAX_SPEED 800.0f

Player player = {0};

const Vector2 playerSpawnPoints[] = {
    (Vector2){5.0f, 153.0f},
};

int framesSincelastBump = 0;

void InitPlayer()
{
    float tileSize = (float)gameMapData.tileSize;
    float tileScale = (float)gameMapData.tileScale;

    float size = tileSize * tileScale;

    player.rect = (Rectangle){
        playerSpawnPoints[gameMapData.currentMapIndex].x * size,
        playerSpawnPoints[gameMapData.currentMapIndex].y * size,
        size,
        size};

    player.vel = (Vector2){0, 0};
    player.thrust = (Vector2){0, -500.0f};
    player.angle = 0.0f;
    player.rotationSpeed = 300.0f;

    player.radius = size * 0.25f;

    player.activeAnimation = PLAYER_IDLE;
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

bool CircleRectCollision(Vector2 c, float r, Rectangle rect)
{
    float closestX = fmaxf(rect.x, fminf(c.x, rect.x + rect.width));
    float closestY = fmaxf(rect.y, fminf(c.y, rect.y + rect.height));

    float dx = c.x - closestX;
    float dy = c.y - closestY;

    return (dx * dx + dy * dy) <= (r * r);
}

bool CheckCollisionWithTiles(Vector2 center, float radius)
{
    Map *currentMap = &gameMapData.activeMap;
    if (!currentMap || currentMap->layerCount <= 0)
        return false;

    float tileSize = gameMapData.tileSize * gameMapData.tileScale;

    for (int i = 0; i < currentMap->layerCount; i++)
    {
        if (!currentMap->layers[i].collisions)
            continue;

        Layer *layer = &currentMap->layers[i];

        for (int row = 0; row < currentMap->gridRows; row++)
        {
            for (int col = 0; col < currentMap->gridCols; col++)
            {
                Tile *t = &layer->tiles[row][col];
                if (!t->indices || t->indiceCount <= 0)
                    continue;

                Rectangle tileRect = {
                    col * tileSize,
                    row * tileSize,
                    tileSize,
                    tileSize};

                if (CircleRectCollision(center, radius, tileRect))
                    return true;
            }
        }
    }
    return false;
}

void UpdatePlayer()
{
    if (!playerFinished)
    {
        float deltaTime = GetFrameTime();
        framesSincelastBump++;

        if (movementActivated)
        {
            if (IsKeyDown(KEY_W))
            {
                player.vel = Vector2Add(
                    player.vel,
                    Vector2Scale(rotate(player.thrust, player.angle, (Vector2){0, 0}), deltaTime));
                player.activeAnimation = PLAYER_FLY;
            }
            else
                player.activeAnimation = PLAYER_IDLE;
        }

        if (IsKeyDown(KEY_D))
            player.angle += deltaTime * player.rotationSpeed;
        if (IsKeyDown(KEY_A))
            player.angle -= deltaTime * player.rotationSpeed;

        float speed = Vector2Length(player.vel);
        if (speed > MAX_SPEED)
            player.vel = Vector2Scale(player.vel, MAX_SPEED / speed);

        Vector2 center = (Vector2){player.rect.x, player.rect.y};

        Vector2 nextX = {center.x + player.vel.x * deltaTime, center.y};
        if (!CheckCollisionWithTiles(nextX, player.radius))
        {
            player.rect.x = nextX.x;
        }
        else
        {
            int sign = (player.vel.x > 0) ? -1 : 1;
            for (int i = 0; i < 5; i++)
            {
                nextX.x += sign * 0.1f;
                if (!CheckCollisionWithTiles(nextX, player.radius))
                {
                    player.rect.x = nextX.x;
                    break;
                }
            }
            player.vel.x *= -0.25f;
        }

        Vector2 nextY = {player.rect.x, center.y + player.vel.y * deltaTime};
        if (!CheckCollisionWithTiles(nextY, player.radius))
        {
            player.rect.y = nextY.y;
        }
        else
        {
            int sign = (player.vel.y > 0) ? -1 : 1;
            for (int i = 0; i < 5; i++)
            {
                nextY.y += sign * 0.1f;
                if (!CheckCollisionWithTiles(nextY, player.radius))
                {
                    player.rect.y = nextY.y;
                    break;
                }
            }
            player.vel.y *= -0.25f;
        }
    }
}

void DrawPlayer()
{
    DrawAnimationAt(
        player.activeAnimation,
        player.rect,
        player.angle,
        1.0f);
}
