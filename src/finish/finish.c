#include "finish.h"

Finish finish;

float distance = 250.0f;

static Animation LoadFinishAnimation(const char *path, int frameCount, float fps)
{
    Animation a = {0};
    a.texture = LoadTexture(path);

    if (a.texture.id == 0) return a;

    a.frameCount = frameCount;
    a.currentFrame = 0;
    a.fps = fps;
    a.frameTimer = 0;

    a.frameRect = (Rectangle){
        0, 0,
        a.texture.width / frameCount,
        a.texture.height
    };

    return a;
}

void InitFinish()
{
    int trueTileSize = gameMapData.tileSize * gameMapData.tileScale;

    finish.rect.x = 28 * trueTileSize;
    finish.rect.y = 4 * trueTileSize;
    finish.rect.width  = 16 * trueTileSize;
    finish.rect.height = 8  * trueTileSize;

    animations[FINISH_IDLE_TOP]       = LoadFinishAnimation(FINISH_IDLE_TOP_PATH, 4, 4);
    animations[FINISH_IDLE_BOTTOM]    = LoadFinishAnimation(FINISH_IDLE_BOTTOM_PATH, 1,0);
    animations[FINISH_OPEN_TOP]       = LoadFinishAnimation(FINISH_OPEN_TOP_PATH, 4, 2);
    animations[FINISH_OPEN_BOTTOM]    = LoadFinishAnimation(FINISH_OPEN_BOTTOM_PATH, 1, 0);
    animations[FINISH_TRANSITION_TOP] = LoadFinishAnimation(FINISH_TRANSITION_TOP_PATH, 8, 16);
    animations[FINISH_TRANSITION_BOTTOM] = LoadFinishAnimation(FINISH_TRANSITION_BOTTOM_PATH, 8, 16);

    finish.animTopID = FINISH_IDLE_TOP;
    finish.animBottomID = FINISH_IDLE_BOTTOM;
}

void UpdateFinish()
{
    Vector2 playerPos = (Vector2){ player.rect.x, player.rect.y };
    Vector2 finishPos = (Vector2){ finish.rect.x, finish.rect.y };
    
    float dist = Vector2Distance(playerPos, finishPos);

    Animation *topAnim    = &animations[finish.animTopID];
    Animation *bottomAnim = &animations[finish.animBottomID];

    if (dist < distance)
    {
        if (finish.animTopID != FINISH_TRANSITION_TOP &&
            finish.animTopID != FINISH_OPEN_TOP)
        {
            finish.animTopID = FINISH_TRANSITION_TOP;
            finish.animBottomID = FINISH_TRANSITION_BOTTOM;

            topAnim->currentFrame = 0;
            topAnim->frameTimer = 0;

            bottomAnim->currentFrame = 0;
            bottomAnim->frameTimer = 0;
        }
    }

    if (finish.animTopID == FINISH_TRANSITION_TOP)
    {
        topAnim = &animations[finish.animTopID];

        if (topAnim->currentFrame >= topAnim->frameCount - 1)
        {
            finish.animTopID    = FINISH_OPEN_TOP;
            finish.animBottomID = FINISH_OPEN_BOTTOM;

            animations[finish.animTopID].currentFrame = 0;
            animations[finish.animTopID].frameTimer   = 0;

            animations[finish.animBottomID].currentFrame = 0;
            animations[finish.animBottomID].frameTimer   = 0;
        }
    }
}


void DrawFinishTop()
{
    DrawAnimationAt(finish.animTopID,    finish.rect, 0.0f, 1.0f);
}

void DrawFinishBottom()
{
    DrawAnimationAt(finish.animBottomID, finish.rect, 0.0f, 1.0f);

}
