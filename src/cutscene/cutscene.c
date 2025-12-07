#include "cutscene.h"

CutsceneSlide slides[3];
int cutsceneIndex = 0;

float cutsceneTimer = 0.0f;
float bgFade = 0.0f;
float textFade = 0.0f;
float textFade2 = 0.0f;

const float FADE_DURATION = 1.3f;
const float HOLD_DURATION = 2.0f;

Text2 text2[3] = {
    {"Assimilation succesful", {900, 700}, (Color){0xF6, 0xD6, 0xBD, 255}},
    {"Find your way back to the mother", {400, 975}, (Color){0xF6, 0xD6, 0xBD, 255}},
    {"And become her new favourite.", {400, 1000}, (Color){0x20, 0x39, 0x4f, 255}}
};

CutState cutState = CS_FADE_IN_BG;

void InitCutscene()
{
    slides[0].bg = LoadTexture(CUTSCENE_BG_1_PATH);
    slides[0].text = LoadTexture(CUTSCENE_TEXT_1_PATH);

    slides[1].bg = LoadTexture(CUTSCENE_BG_2_PATH);
    slides[1].text = LoadTexture(CUTSCENE_TEXT_2_PATH);

    slides[2].bg = LoadTexture(CUTSCENE_BG_3_PATH);
    slides[2].text = LoadTexture(CUTSCENE_TEXT_3_PATH);

}

void UpdateCutscene()
{

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        gameState = MENU;
        return;
    }
    float dt = GetFrameTime();
    cutsceneTimer += dt;

    switch (cutState)
    {

    case CS_FADE_IN_BG:
        bgFade = cutsceneTimer / FADE_DURATION;
        if (bgFade >= 1.0f)
        {
            bgFade = 1.0f;
            cutsceneTimer = 0.0f;
            cutState = CS_FADE_IN_TEXT;
        }
        break;

    case CS_FADE_IN_TEXT:
        textFade = cutsceneTimer / FADE_DURATION;
        if (textFade >= 1.0f)
        {
            textFade = 1.0f;
            cutsceneTimer = 0.0f;
            cutState = CS_FADE_IN_TEXT_2;
        }
        break;

        case CS_FADE_IN_TEXT_2:
        textFade2 = cutsceneTimer / FADE_DURATION;
        if (textFade2 >= 1.0f)
        {
            textFade2 = 1.0f;
            cutsceneTimer = 0.0f;
            cutState = CS_HOLD;
        }
        break;

    case CS_HOLD:
        if (cutsceneTimer >= HOLD_DURATION)
        {
            cutsceneTimer = 0.0f;
            cutState = CS_FADE_OUT;
        }
        break;

    case CS_FADE_OUT:
        bgFade = 1.0f - (cutsceneTimer / FADE_DURATION);
        textFade = 1.0f - (cutsceneTimer / FADE_DURATION);
        textFade2 = 1.0f - (cutsceneTimer / FADE_DURATION);
        if (cutsceneTimer >= FADE_DURATION)
        {
            bgFade = 0.0f;
            textFade = 0.0f;
            textFade2 = 0.0f;
            cutsceneTimer = 0.0f;
            cutsceneIndex++;
            cutState = (cutsceneIndex >= 3) ? CS_DONE : CS_FADE_IN_BG;
            if (cutsceneIndex >= 3)
                gameState = MENU;
        }
        break;

    case CS_DONE:
        gameState = MENU;
        break;
    }
}

void DrawCutscene()
{

    CutsceneSlide *s = &slides[cutsceneIndex];

    Color bgCol = WHITE; bgCol.a = (unsigned char)(255 * bgFade);

    DrawTexture(s->bg, 0, 0, bgCol);

    Color txtCol = (Color){0xF6, 0xD6, 0xBD, 255};
    txtCol.a = (unsigned char)(255 * textFade);

    DrawTexture(s->text, 0, 0, txtCol);

    text2[cutsceneIndex].color.a = (unsigned char)(255 * textFade2);

    DrawText(text2[cutsceneIndex].text, text2[cutsceneIndex].position.x, text2[cutsceneIndex].position.y, 60, text2[cutsceneIndex].color);
}

void UnloadCutscene()
{
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(slides[i].bg);
        UnloadTexture(slides[i].text);
    }
}
