#include "maps.h"

GameMapData gameMapData = {0};

void DecompressRow(cJSON *rowJson, Tile *outRow, int cols)
{
    int col = 0;

    for (cJSON *seg = rowJson->child; seg && col < cols; seg = seg->next)
    {

        int count = 1;
        cJSON *value = seg;

        if (
            cJSON_IsArray(seg) &&
            cJSON_GetArraySize(seg) == 2 &&
            cJSON_IsNumber(cJSON_GetArrayItem(seg, 0))
        ) {
            count = cJSON_GetArrayItem(seg, 0)->valueint;
            value = cJSON_GetArrayItem(seg, 1);
        }


        for (int i = 0; i < count && col < cols; i++, col++)
        {
            Tile *tile = &outRow[col];
            tile->indices = NULL;
            tile->indiceCount = 0;
            tile->currentFrame = 0;
            tile->animationTimer = 0;

            if (cJSON_IsArray(value))
            {
                int n = cJSON_GetArraySize(value);
                tile->indiceCount = n;
                tile->indices = malloc(sizeof(int) * n);

                int k = 0;
                for (cJSON *idx = value->child; idx; idx = idx->next, k++)
                {
                    tile->indices[k] = idx->valueint;
                }
            }
        }
    }
}

void DecompressRows(cJSON *rowsJson, Tile **tiles, int cols, int rows)
{
    int row = 0;

    for (cJSON *seg = rowsJson->child; seg && row < rows; seg = seg->next)
    {

        int count = 1;
        cJSON *rowValue = seg;

        if (
            cJSON_IsArray(seg) &&
            cJSON_GetArraySize(seg) == 2 &&
            cJSON_IsNumber(cJSON_GetArrayItem(seg, 0)) &&
            cJSON_IsArray(cJSON_GetArrayItem(seg, 1))
        ) 
    {
    count = cJSON_GetArrayItem(seg, 0)->valueint;
    rowValue = cJSON_GetArrayItem(seg, 1);
}


        for (int i = 0; i < count && row < rows; i++, row++)
        {
            DecompressRow(rowValue, tiles[row], cols);
        }
    }
}

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

    int i = 0;
    for (cJSON *layerJson = layersJson->child; layerJson; layerJson = layerJson->next, i++)
    {
        layers[i].opacity =
            cJSON_GetObjectItem(layerJson, "opacity") ? (float)cJSON_GetObjectItem(layerJson, "opacity")->valuedouble : 1.0f;

        layers[i].collisions =
            cJSON_IsTrue(cJSON_GetObjectItem(layerJson, "collisions"));

        layers[i].tiles = CreateTileGrid(cols, rows);

        cJSON *tiles = cJSON_GetObjectItem(layerJson, "tiles");
        if (cJSON_IsArray(tiles))
        {
            DecompressRows(tiles, layers[i].tiles, cols, rows);
        }
    }
    return layers;
}

Map ParseMap(cJSON *mapJson)
{
    Map map = {0};

    map.gridCols = cJSON_GetObjectItem(mapJson, "gridCols")->valueint;
    map.gridRows = cJSON_GetObjectItem(mapJson, "gridRows")->valueint;

    cJSON *layers = cJSON_GetObjectItem(mapJson, "layers");
    map.layers = ParseLayers(layers, map.gridCols, map.gridRows, &map.layerCount);

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
    gameMapData.tileScale = 2;
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
