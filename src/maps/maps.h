#ifndef MAPS_H
#define MAPS_H

#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>
#include "raylib.h"
#include "globals.h"

typedef struct {
    int *indices;
    int currentFrame;
    int indiceCount;
    double animationTimer;
} Tile;

typedef struct {
    float opacity;
    bool collisions;
    Tile **tiles;
} Layer;

typedef struct {
    int gridCols;
    int gridRows;
    Layer *layers;
    int layerCount;
} Map;

typedef struct {
    int tileSize;
    int animationSpeed;
    int tileScale;
    Map *maps;
    Map activeMap;
    int mapCount;
    int currentMapIndex;
    Texture2D tilesetTexture;
} GameMapData;

extern GameMapData gameMapData;

void InitMaps();
void LoadMap(int mapIndex);
void DrawCurrentMap();

#endif
