#include "maps.h"

GameMapData gameMapData = {0};

Tile **CreateTileGrid(int cols, int rows)
{
    Tile **tiles = malloc(rows * sizeof(Tile *));
    for (int i = 0; i < rows; i++)
    {
        tiles[i] = calloc(cols, sizeof(Tile));
        for (int c = 0; c < cols; c++)
        {
            tiles[i][c].indices = NULL;
        }
    }
    return tiles;
}

void ParseTiles(cJSON *tilesJson, Tile **tiles, int cols, int rows)
{
    if (!tilesJson)
        return;

    int row = 0;
    for (cJSON *rowJson = tilesJson->child; rowJson && row < rows; rowJson = rowJson->next, row++)
    {
        if (!cJSON_IsArray(rowJson))
            continue;

        int col = 0;
        for (cJSON *tileJson = rowJson->child; tileJson && col < cols; tileJson = tileJson->next, col++)
        {
            tiles[row][col].indices = NULL;
            tiles[row][col].indiceCount = 0;
            tiles[row][col].animationTimer = 0;
            tiles[row][col].currentFrame = 0;

            if (cJSON_IsObject(tileJson))
            {
                cJSON *indicesArray = cJSON_GetObjectItem(tileJson, "indices");
                if (indicesArray && cJSON_IsArray(indicesArray))
                {
                    int count = cJSON_GetArraySize(indicesArray);

                    tiles[row][col].indiceCount = count;
                    tiles[row][col].indices = malloc(count * sizeof(int));

                    int i = 0;
                    for (cJSON *idx = indicesArray->child; idx; idx = idx->next, i++)
                    {
                        tiles[row][col].indices[i] = idx->valueint;
                    }
                }
            }
        }
    }
}


Layer *ParseLayers(cJSON *layersJson, int cols, int rows, int *layerCount)
{
    *layerCount = cJSON_GetArraySize(layersJson);
    Layer *layers = malloc(*layerCount * sizeof(Layer));

    int layerIndex = 0;
    for (cJSON *layerJson = layersJson->child; layerJson; layerJson = layerJson->next, layerIndex++)
    {
        cJSON *opacity = cJSON_GetObjectItem(layerJson, "opacity");
        cJSON *collisions = cJSON_GetObjectItem(layerJson, "collisions");
        cJSON *tiles = cJSON_GetObjectItem(layerJson, "tiles");

        layers[layerIndex].opacity = opacity ? opacity->valuedouble : 1.0f;
        layers[layerIndex].collisions = collisions ? collisions->type == cJSON_True : false;
        layers[layerIndex].tiles = CreateTileGrid(cols, rows);
        ParseTiles(tiles, layers[layerIndex].tiles, cols, rows);
    }
    return layers;
}

Map ParseMap(cJSON *mapJson)
{
    Map map = {0};

    cJSON *cols = cJSON_GetObjectItem(mapJson, "gridCols");
    cJSON *rows = cJSON_GetObjectItem(mapJson, "gridRows");
    cJSON *layers = cJSON_GetObjectItem(mapJson, "layers");

    map.gridCols = cols ? cols->valueint : 0;
    map.gridRows = rows ? rows->valueint : 0;
    map.layers = cJSON_IsArray(layers) ? ParseLayers(layers, map.gridCols, map.gridRows, &map.layerCount) : NULL;

    return map;
}

void InitMaps()
{
    char *json = LoadFileText(MAP_JSON_PATH);
    cJSON *root = cJSON_Parse(json);

    if (!root)
    {
        printf("Failed to parse maps.json\n");
        return;
    }

    cJSON *tileSize = cJSON_GetObjectItem(root, "tileSize");
    cJSON *animationSpeed = cJSON_GetObjectItem(root, "animationSpeed");
    cJSON *maps = cJSON_GetObjectItem(root, "maps");

    gameMapData.tileSize = tileSize ? tileSize->valueint : 16;
    gameMapData.animationSpeed = animationSpeed ? animationSpeed->valueint : 8;
    gameMapData.tileScale = 1;
    gameMapData.mapCount = cJSON_GetArraySize(maps);
    gameMapData.maps = malloc(gameMapData.mapCount * sizeof(Map));
    gameMapData.currentMapIndex = 0;

    int i = 0;
    for (cJSON *mapJson = maps->child; mapJson; mapJson = mapJson->next, i++)
    {
        gameMapData.maps[i] = ParseMap(mapJson);
    }

    gameMapData.tilesetTexture = LoadTexture(TILESET_PATH);

    LoadMap(gameMapData.currentMapIndex);

    cJSON_Delete(root);
    UnloadFileText(json);
}

void LoadMap(int index)
{
    if (index < 0 || index >= gameMapData.mapCount)
    {
        printf("Invalid map index: %d\n", index);
        return;
    }
    gameMapData.currentMapIndex = index;
    gameMapData.activeMap = gameMapData.maps[index];
}

void DrawCurrentMap()
{
    Map *currentMap = &gameMapData.activeMap;
    if (!currentMap || currentMap->layerCount <= 0)
        return;

    Texture2D tilesetTexture = gameMapData.tilesetTexture;
    if (tilesetTexture.id == 0)
        return;

    int tilesPerRow = tilesetTexture.width / gameMapData.tileSize;
    float tileSizeF = (float)gameMapData.tileSize;
    float scale = (float)gameMapData.tileScale;

    double frameDuration = 1.0 / gameMapData.animationSpeed;
    double deltaTime = GetFrameTime();

    for (int j = 0; j < currentMap->layerCount; j++)
    {
        Layer *layer = &currentMap->layers[j];

        for (int k = 0; k < currentMap->gridRows; k++)
        {
            for (int l = 0; l < currentMap->gridCols; l++)
            {
                Tile *tile = &layer->tiles[k][l];

                if (!tile->indices || tile->indiceCount <= 0)
                    continue;

                tile->animationTimer += deltaTime;

                while (tile->animationTimer >= frameDuration)
                {
                    tile->animationTimer -= frameDuration;
                    tile->currentFrame = (tile->currentFrame + 1) % tile->indiceCount;
                }

                int tileIndex = tile->indices[tile->currentFrame];

                int sourceX = (tileIndex % tilesPerRow) * gameMapData.tileSize;
                int sourceY = (tileIndex / tilesPerRow) * gameMapData.tileSize;

                Rectangle src = { (float)sourceX, (float)sourceY, tileSizeF, tileSizeF };
                Rectangle dst = {
                    (float)l * tileSizeF * scale,
                    (float)k * tileSizeF * scale,
                    tileSizeF * scale,
                    tileSizeF * scale
                };

                Color tint = (Color){255, 255, 255, (unsigned char)(255.0f * layer->opacity)};
                DrawTexturePro(tilesetTexture, src, dst, (Vector2){0, 0}, 0.0f, tint);
            }
        }
    }
}
