#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const int BUTTON_HEIGHT = 100;
int defaultFontSize = 64;

float timeUntilCountdown = 2.4f;
float countdownTimer = 0.0f;
int countdownIndex = -1;
const char* countdownTexts[] = {"3", "2", "1", "GO"};
const int countdownCount = 4;
float countdownInterval = 0.8f; 

float fadeTimer = 0.0f;
float fadeDuration = 0.6f;

bool countdownStarted = false;
bool countdownFinished = false;

void InitGUI()
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xF6D6BDFF);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 16);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x977577FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x816271FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x4E495FFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xF6D6BDFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xF6D6BDFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xF6D6BDFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0xF6D6BDFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0xF6D6BDFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0xF6D6BDFF);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 4);
}

void RenderMenuGUI()
{
    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH/2 - panelWidth/2;

    int y = 250;
    int spacing = 50; 

    GuiSetStyle(DEFAULT, TEXT_SIZE, 128);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xF6D6BDFF);
    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 30 }, "Alien Race");
    y += spacing * 2;

    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xC3A38AFF);
    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 20 }, "Assimilation");
    y += spacing * 3;

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xF6D6BDFF);

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Play")) {
        gameState = LOBBY_SELECTOR;
    }
    y += BUTTON_HEIGHT;
    y += spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Credits")) {
        gameState = CREDITS;
    }
    y += BUTTON_HEIGHT;
    y += spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Exit")) {
        UnloadGame();
        CloseWindow();
        exit(0);
    }
}

void RenderLobbySelectorGUI()
{
    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH/2 - panelWidth/2;

    int y = 250;
    int spacing = 50;

    GuiLabel((Rectangle){ panelX, y, panelWidth, 30 }, "Select Lobby");
    y += spacing * 2;

    if (GuiButton((Rectangle){GAME_WIDTH - 350, 50, 300, BUTTON_HEIGHT}, "Back")) {
        gameState = MENU;
    }

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT }, "Lobby 1")) {
        gameState = GAME;
    }
}

void RenderCreditsGUI(){
    
}

void RenderInGameGUI()
{
}

void StartCountdown()
{
    countdownStarted = false;
    countdownFinished = false;
    countdownTimer = 0.0f;
    fadeTimer = 0.0f;
    countdownIndex = -1;
}

void UpdateCountdown()
{
    if (countdownFinished) return;

    float dt = GetFrameTime();
    countdownTimer += dt;

    if (!countdownStarted)
    {
        if (countdownTimer >= timeUntilCountdown)
        {
            countdownStarted = true;
            countdownTimer = 0.0f;
            countdownIndex = 0;
            fadeTimer = 0.0f;

            SeekMusicStream(countdownMusic, 0);
            PlayMusicStream(countdownMusic);
        }
        return;
    }

    fadeTimer += dt;

    if (countdownTimer >= countdownInterval)
    {
        countdownTimer = 0.0f;
        fadeTimer = 0.0f;

        countdownIndex++;
        if (countdownIndex >= countdownCount)
        {
            countdownFinished = true;
        }
    }

    if (countdownFinished && !IsMusicStreamPlaying(countdownMusic))
    {
        SeekMusicStream(gameMusic, 0);
        PlayMusicStream(gameMusic);
    }
}


void DrawCountdown()
{
    if (!countdownStarted || countdownFinished || countdownIndex < 0) return;

    const char* text = countdownTexts[countdownIndex];

    float opacity = 1.0f - (fadeTimer / fadeDuration);
    if (opacity < 0) opacity = 0;

    int fontSize = 180;

    Vector2 size = MeasureTextEx(GetFontDefault(), text, fontSize, 5);
    Vector2 pos = { GAME_WIDTH / 2 - size.x / 2, GAME_HEIGHT / 2 - size.y / 2 };

    Color c = WHITE;
    c.a = (unsigned char)(opacity * 255);

    DrawTextEx(GetFontDefault(), text, pos, fontSize, 5, c);
}