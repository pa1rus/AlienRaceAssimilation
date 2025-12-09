#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const int BUTTON_HEIGHT = 100;
int defaultFontSize = 64;

float timeUntilCountdown = 2.4f;
float countdownTimer = 0.0f;
int countdownIndex = -1;
const char *countdownTexts[] = {"3", "2", "1", "GO"};
const int countdownCount = 4;
float countdownInterval = 0.8f;

float fadeTimer = 0.0f;
float fadeDuration = 0.6f;

bool countdownStarted = false;
bool countdownFinished = false;

bool movementActivated = false;
float movementTimer = 0.0f;

bool endMenuActive = false;
float endMenuAlpha = 0.0f;
float endMenuFadeSpeed = 2.0f;

float lastRunTime = 0.0f;
float bestTime;

Texture2D kl;
Texture2D mb;
Texture2D jb;

Color text;

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

    GuiSetStyle(TEXTBOX, TEXT_PADDING, 32);
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 4);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, 0xF6D6BDFF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0xF6D6BDFF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, 0xF6D6BDFF);
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0x4E495FFF);
    GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED, 0x4E495FFF);
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, 0x4E495FFF);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xF6D6BDFF);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, 0xF6D6BDFF);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, 0xF6D6BDFF);

    kl = LoadTexture(KL_PATH);
    mb = LoadTexture(MB_PATH);
    jb = LoadTexture(JB_PATH);

    text = (Color){0xF6, 0xD6, 0xBD, 255};
}

void RenderMenuGUI()
{
    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH / 2 - panelWidth / 2;

    int y = 250;
    int spacing = 50;

    GuiSetStyle(DEFAULT, TEXT_SIZE, 128);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xF6D6BDFF);
    GuiLabel((Rectangle){0, y, GAME_WIDTH, 30}, "Alien Race");
    y += spacing * 2;

    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xC3A38AFF);
    GuiLabel((Rectangle){0, y, GAME_WIDTH, 20}, "Assimilation");
    y += spacing * 3;

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xF6D6BDFF);

    if (GuiButton((Rectangle){panelX, y, panelWidth, BUTTON_HEIGHT}, "Play"))
    {
        gameState = GAME;
    }
    y += BUTTON_HEIGHT;
    y += spacing;

    if (GuiButton((Rectangle){panelX, y, panelWidth, BUTTON_HEIGHT}, "Credits"))
    {
        gameState = CREDITS;
    }
    y += BUTTON_HEIGHT;
    y += spacing;

    if (GuiButton((Rectangle){panelX, y, panelWidth, BUTTON_HEIGHT}, "Exit"))
    {
        UnloadGame();
        CloseWindow();
        exit(0);
    }
    if (bestTime > 0.0f)
    {
        char buf[64];
        char timeBuf[32];
        FormatTime(timeBuf, sizeof(timeBuf), bestTime);

        snprintf(buf, sizeof(buf), "Best time: %s", timeBuf);

        Vector2 size = MeasureTextEx(GetFontDefault(), buf, 32, 0);
        DrawText(buf, GAME_WIDTH / 2 - size.x / 2, 975, 32, text);
    }
}

void RenderCreditsGUI()
{
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    if (GuiButton((Rectangle){GAME_WIDTH - 350, 50, 300, BUTTON_HEIGHT}, "Back"))
        gameState = MENU;

    int colSpacing = 80;
    int startY = 300;
    float multiplier = 5.0f;

    int spacingText = 30;

    int y = startY;
    int imgW = kl.width * multiplier;
    int imgH = kl.height * multiplier;
    int colWidth = imgW + 40;
    int totalWidth = colWidth * 3 + colSpacing * 2;
    int startX = GAME_WIDTH / 2 - totalWidth / 2;

    int centerX = startX + colWidth / 2;

    Vector2 size = MeasureTextEx(GetFontDefault(), "Krzysztof L.", 32, 0);
    DrawText("Krzysztof L.", centerX - size.x / 2, y, 32, text);
    y += spacingText;

    DrawTexturePro(
        kl,
        (Rectangle){0, 0, kl.width, kl.height},
        (Rectangle){centerX - imgW / 2, y, imgW, imgH},
        (Vector2){0, 0},
        0,
        WHITE);
    y += imgH + spacingText;

    size = MeasureTextEx(GetFontDefault(), "Client Logic & Sounds", 24, 0);
    DrawText("Client Logic & Sounds", centerX - size.x / 2, y, 24, text);

    y = startY;
    int col2X = startX + colWidth + colSpacing;
    centerX = col2X + colWidth / 2;

    size = MeasureTextEx(GetFontDefault(), "Mikolaj B.", 32, 0);
    DrawText("Mikolaj B.", centerX - size.x / 2, y, 32, text);
    y += spacingText;

    imgW = mb.width * multiplier;
    imgH = mb.height * multiplier;
    DrawTexturePro(
        mb,
        (Rectangle){0, 0, mb.width, mb.height},
        (Rectangle){centerX - imgW / 2, y, imgW, imgH},
        (Vector2){0, 0},
        0,
        WHITE);
    y += imgH + spacingText;

    size = MeasureTextEx(GetFontDefault(), "Graphics & Level Design", 24, 0);
    DrawText("Graphics & Level Design", centerX - size.x / 2, y, 24, text);

    y = startY;
    int col3X = startX + (colWidth + colSpacing) * 2;
    centerX = col3X + colWidth / 2;

    size = MeasureTextEx(GetFontDefault(), "Jan B.", 32, 0);
    DrawText("Jan B.", centerX - size.x / 2, y, 32, text);
    y += spacingText;

    imgW = jb.width * multiplier;
    imgH = jb.height * multiplier;
    DrawTexturePro(
        jb,
        (Rectangle){0, 0, jb.width, jb.height},
        (Rectangle){centerX - imgW / 2, y, imgW, imgH},
        (Vector2){0, 0},
        0,
        WHITE);
    y += imgH + spacingText;

    size = MeasureTextEx(GetFontDefault(), "Server", 24, 0);
    DrawText("Server", centerX - size.x / 2, y, 24, text);
}

void UpdateInGameGUI()
{
    UpdateCountdown();
    UpdateMovementTimer();

    if (endMenuActive && endMenuAlpha < 1.0f)
    {
        endMenuAlpha += GetFrameTime() * endMenuFadeSpeed;
        if (endMenuAlpha > 1.0f)
            endMenuAlpha = 1.0f;
    }
}

void FormatTime(char *buffer, int size, float timeSec)
{
    int minutes = (int)(timeSec / 60.0f);
    int seconds = (int)timeSec % 60;
    int milliseconds = (int)((timeSec - (int)timeSec) * 1000.0f);

    snprintf(buffer, size, "%02d:%02d:%03d", minutes, seconds, milliseconds);
}

void DrawPauseGUI(){

    int w = GAME_WIDTH;
    int h = 400;
    int x = GAME_WIDTH / 2 - w / 2;
    int y = GAME_HEIGHT / 2 - h / 2;

    DrawRectangle(0, 0, GAME_WIDTH, GAME_HEIGHT, (Color){0, 0, 0, 100});

    GuiLabel((Rectangle){x, y + 40, w, 40}, "Pause");

    int bw = 400;
    int bh = 100;
    int bx = GAME_WIDTH / 2 - bw / 2;

    if (GuiButton((Rectangle){bx, y + 120, bw, bh}, "Replay"))
    {
        movementTimer = 0;
        movementActivated = false;
        playerFinished = false;
        endMenuActive = false;
        endMenuAlpha = 0.0f;

        PrepareGame();
        StartGame();
    }

    if (GuiButton((Rectangle){bx, y + 240, bw, bh}, "Menu"))
    {
        gameState = MENU;
        gameStarted = false;
        movementTimer = 0;
        movementActivated = false;
        playerFinished = false;
        endMenuActive = false;
        endMenuAlpha = 0.0f;
        PrepareGame();
        ShowCursor();
    }
    
}

void DrawEndingScreen()
{
    if (!endMenuActive)
        return;

    unsigned char a = (unsigned char)(endMenuAlpha * 200);
    DrawRectangle(0, 0, GAME_WIDTH, GAME_HEIGHT, (Color){0, 0, 0, a});

    int w = GAME_WIDTH;
    int h = 500;
    int x = GAME_WIDTH / 2 - w / 2;
    int y = GAME_HEIGHT / 2 - h / 2;

    char lastBuf[32];
    char bestBuf[32];
    FormatTime(lastBuf, sizeof(lastBuf), lastRunTime);
    FormatTime(bestBuf, sizeof(bestBuf), bestTime);

    char lastLabel[48];
    char bestLabel[48];

    snprintf(lastLabel, sizeof(lastLabel), "Time: %s", lastBuf);
    snprintf(bestLabel, sizeof(bestLabel), "Best: %s", bestBuf);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiLabel((Rectangle){x, y + 40, w, 40}, "Finished!");
    GuiLabel((Rectangle){x, y + 120, w, 40}, lastLabel);
    GuiLabel((Rectangle){x, y + 180, w, 40}, bestLabel);

    int bw = 400;
    int bh = 100;
    int bx = GAME_WIDTH / 2 - bw / 2;

    if (GuiButton((Rectangle){bx, y + 260, bw, bh}, "Replay"))
    {
        movementTimer = 0;
        movementActivated = false;
        playerFinished = false;
        endMenuActive = false;
        endMenuAlpha = 0.0f;

        PrepareGame();
        StartGame();
    }

    if (GuiButton((Rectangle){bx, y + 380, bw, bh}, "Menu"))
    {
        gameState = MENU;
        gameStarted = false;
        movementTimer = 0;
        movementActivated = false;
        playerFinished = false;
        endMenuActive = false;
        endMenuAlpha = 0.0f;
        PrepareGame();
        ShowCursor();
    }
}

void DrawInGameGUI()
{
    DrawCountdown();
    DrawMovementTimer();
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
    if (countdownFinished)
        return;

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
        if (countdownIndex >= countdownCount - 1)
        {
            movementActivated = true;
        }
        if (countdownIndex >= countdownCount)
        {
            countdownFinished = true;
        }
    }
}

void DrawCountdown()
{
    if (!countdownStarted || countdownFinished || countdownIndex < 0)
        return;

    const char *text = countdownTexts[countdownIndex];

    float opacity = 1.0f - (fadeTimer / fadeDuration);
    if (opacity < 0)
        opacity = 0;

    int fontSize = 180;

    Vector2 size = MeasureTextEx(GetFontDefault(), text, fontSize, 5);
    Vector2 pos = {GAME_WIDTH / 2 - size.x / 2, GAME_HEIGHT / 2 - size.y / 2};

    Color c = WHITE;
    c.a = (unsigned char)(opacity * 255);

    DrawTextEx(GetFontDefault(), text, pos, fontSize, 5, c);
}

void UpdateMovementTimer()
{
    if (movementActivated)
        movementTimer += GetFrameTime();
}

void DrawMovementTimer()
{
    if (!movementActivated)
        return;

    char buffer[32];
    FormatTime(buffer, sizeof(buffer), movementTimer);

    int fontSize = 32;
    DrawText(buffer, 20, 20, fontSize, WHITE);
}

void UnloadGUI()
{
    UnloadTexture(kl);
    UnloadTexture(mb);
    UnloadTexture(jb);
}