#include "animations.h"

Animation animations[ANIMATIONS_COUNT];

static Animation LoadAnimation(const char *path, int frames, float fps)
{
    Animation a = {0};
    a.texture = LoadTexture(path);

    if (a.texture.id == 0) {
        return a;
    }

    a.frameCount = frames;
    a.currentFrame = 0;
    a.fps = fps;
    a.frameTimer = 0;

    a.frameRect = (Rectangle){
        0,
        0,
        a.texture.width / frames,
        a.texture.height
    };

    return a;
}

void InitAnimations(void)
{
    animations[PLAYER_IDLE] = LoadAnimation(PLAYER_ANIMATION_IDLE_PATH, 12, 6.0f);
    animations[PLAYER_FLY]  = LoadAnimation(PLAYER_ANIMATION_FLY_PATH,  12, 12.0f);
}

void UpdateAnimations(void)
{
    float dt = GetFrameTime();

    for (int i = 0; i < ANIMATIONS_COUNT; i++)
    {
        Animation *a = &animations[i];
        if (a->texture.id == 0 || a->frameCount <= 1) continue;

        a->frameTimer += dt;

        float frameTime = 1.0f / a->fps;
        if (a->frameTimer >= frameTime)
        {
            a->frameTimer -= frameTime;
            a->currentFrame = (a->currentFrame + 1) % a->frameCount;
            a->frameRect.x = a->currentFrame * a->frameRect.width;
        }
    }
}

void DrawAnimationAt(AnimationID id, Rectangle dest, float angle, float scale)
{
    Animation *a = &animations[id];
    if (a->texture.id == 0) return;

    Rectangle dst = dest;
    dst.width *= scale;
    dst.height *= scale;

    Vector2 origin = { dst.width * 0.5f, dst.height * 0.5f };

    DrawTexturePro(a->texture, a->frameRect, dst, origin, angle, WHITE);
}


void UnloadAnimations(void)
{
    for (int i = 0; i < ANIMATIONS_COUNT; i++)
        if (animations[i].texture.id != 0)
            UnloadTexture(animations[i].texture);
}
