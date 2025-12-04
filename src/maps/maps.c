#include "maps.h"

GameMapData gameMapData = {0};

Tile** CreateTileGrid(int cols, int rows) {
    Tile **tiles = malloc(rows * sizeof(Tile *));
    for (int i = 0; i < rows; i++) {
        tiles[i] = calloc(cols, sizeof(Tile));
        for (int c = 0; c < cols; c++) {
            tiles[i][c].index = -1;
        }
    }
    return tiles;
}

void ParseTiles(cJSON *tilesJson, Tile **tiles, int cols, int rows) {
    if (!tilesJson) return;

    int row = 0;
    for (cJSON *r = tilesJson->child; r && row < rows; r = r->next, row++) {
        if (!cJSON_IsArray(r)) continue;

        int col = 0;
        for (cJSON *t = r->child; t && col < cols; t = t->next, col++) {
            tiles[row][col].index = -1;
            if (cJSON_IsObject(t)) {
                cJSON *idx = cJSON_GetObjectItem(t, "index");
                if (idx && cJSON_IsNumber(idx)) {
                    tiles[row][col].index = idx->valueint;
                }
            }
        }
    }
}

Layer* ParseLayers(cJSON *layersJson, int cols, int rows, int *layerCount) {
    *layerCount = cJSON_GetArraySize(layersJson);
    Layer *layers = malloc(*layerCount * sizeof(Layer));

    int idx = 0;
    for (cJSON *l = layersJson->child; l; l = l->next, idx++) {
        cJSON *opacity = cJSON_GetObjectItem(l, "opacity");
        cJSON *collisions = cJSON_GetObjectItem(l, "collisions");
        cJSON *tiles = cJSON_GetObjectItem(l, "tiles");

        layers[idx].opacity = opacity ? opacity->valuedouble : 1.0f;
        layers[idx].collisions = collisions ? collisions->type == cJSON_True : false;
        layers[idx].tiles = CreateTileGrid(cols, rows);
        ParseTiles(tiles, layers[idx].tiles, cols, rows);
    }
    return layers;
}

Map ParseMap(cJSON *mapJson) {
    Map map = {0};
    
    cJSON *cols = cJSON_GetObjectItem(mapJson, "gridCols");
    cJSON *rows = cJSON_GetObjectItem(mapJson, "gridRows");
    cJSON *layers = cJSON_GetObjectItem(mapJson, "layers");

    map.gridCols = cols ? cols->valueint : 0;
    map.gridRows = rows ? rows->valueint : 0;
    map.layers = cJSON_IsArray(layers) ? ParseLayers(layers, map.gridCols, map.gridRows, &map.layerCount) : NULL;

    return map;
}

void InitMaps() {
    char *json = LoadFileText(MAP_JSON_PATH);
    cJSON *root = cJSON_Parse(json);

    if (!root) {
        printf("Failed to parse maps.json\n");
        return;
    }

    cJSON *tileSize = cJSON_GetObjectItem(root, "tileSize");
    cJSON *maps = cJSON_GetObjectItem(root, "maps");

    gameMapData.tileSize = tileSize ? tileSize->valueint : 16;
    gameMapData.tileScale = 3;
    gameMapData.mapCount = cJSON_GetArraySize(maps);
    gameMapData.maps = malloc(gameMapData.mapCount * sizeof(Map));
    gameMapData.currentMapIndex = 1;

    int i = 0;
    for (cJSON *m = maps->child; m; m = m->next, i++) {
        gameMapData.maps[i] = ParseMap(m);
    }

    gameMapData.tilesetTexture = LoadTexture(TILESET_PATH);

    LoadMap(gameMapData.currentMapIndex);

    cJSON_Delete(root);
    UnloadFileText(json);
}

void LoadMap(int index) {
    if (index < 0 || index >= gameMapData.mapCount) {
        printf("Invalid map index: %d\n", index);
        return;
    }
    gameMapData.currentMapIndex = index;
    gameMapData.activeMap = gameMapData.maps[index];
}

void DrawCurrentMap() {
    Map *m = &gameMapData.activeMap;
    if (!m || m->layerCount <= 0) return;
    Texture2D tex = gameMapData.tilesetTexture;
    if (tex.id == 0) return;

    int tilesPerRow = tex.width / gameMapData.tileSize;
    float tileSizeF = (float)gameMapData.tileSize;
    float scale = (float)gameMapData.tileScale;

    for (int li = 0; li < m->layerCount; li++) {
        Layer *layer = &m->layers[li];
        for (int r = 0; r < m->gridRows; r++) {
            for (int c = 0; c < m->gridCols; c++) {
                int tid = layer->tiles[r][c].index;
                if (tid < 0) continue;

                int sx = (tid % tilesPerRow) * gameMapData.tileSize;
                int sy = (tid / tilesPerRow) * gameMapData.tileSize;
                Rectangle src = { (float)sx, (float)sy, tileSizeF, tileSizeF };

                Rectangle dst = {
                    (float)c * tileSizeF * scale,
                    (float)r * tileSizeF * scale,
                    tileSizeF * scale,
                    tileSizeF * scale
                };

                Color tint = (Color){255,255,255,(unsigned char)(255.0f * layer->opacity)};
                DrawTexturePro(tex, src, dst, (Vector2){0,0}, 0.0f, tint);
            }
        }
    }
}
