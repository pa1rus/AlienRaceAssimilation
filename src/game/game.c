#include "game.h"
#include "maps.h"

void InitGame(){
    InitMaps();
}

void UpdateGame(){

}

void DrawGame(){

    BeginDrawing();
    ClearBackground(BLACK);

    DrawCurrentMap();

    EndDrawing();
}

void UpdateDrawFrame(){
    UpdateGame();
    DrawGame();
}

void UnloadGame(){
    
    if (gameMapData.tilesetTexture.id != 0) UnloadTexture(gameMapData.tilesetTexture);
}
