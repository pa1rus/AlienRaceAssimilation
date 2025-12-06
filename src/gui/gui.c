#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

float lobbyScrollY = 0.0f;
Lobby lobbies[] = {
    {"Lobby 1"}, {"Lobby 2"}, {"Lobby 3"}, {"Lobby 4"}, {"Lobby 5"}, {"Lobby 6"}, {"Lobby 7"}, {"Lobby 8"}};
int lobbyCount = sizeof(lobbies) / sizeof(lobbies[0]);

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

char lobbyName[16] = "";

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
        gameState = LOBBY_SELECTOR;
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
}

void RenderLobbySelectorGUI()
{
    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH / 2 - panelWidth / 2;

    int titleY = 150;
    int spacing = 20;
    float btnH = (float)BUTTON_HEIGHT;

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel((Rectangle){panelX, titleY, panelWidth, 40}, "Select Lobby");

    if (GuiButton((Rectangle){GAME_WIDTH - 350, 50, 300, BUTTON_HEIGHT}, "Back"))
    {
        gameState = MENU;
    }

    Rectangle view = {
        (float)panelX,
        (float)(titleY + 120),
        (float)panelWidth,
        520.0f};

    Color panelBg = (Color){0x20, 0x39, 0x4f, 0xFF};
    DrawRectangleRec(view, panelBg);

    float contentHeight = lobbyCount * (btnH + spacing);

    float maxScroll = fmaxf(0.0f, contentHeight - view.height);

    Rectangle scrollbarArea = {
        view.x + view.width - 16.0f,
        view.y,
        16.0f,
        view.height};

    int sbValue = (int)lobbyScrollY;
    sbValue = GuiScrollBar(scrollbarArea, sbValue, 0, (int)maxScroll);
    lobbyScrollY = (float)sbValue;

    if (CheckCollisionPointRec(GetMousePosition(), view))
    {
        lobbyScrollY -= GetMouseWheelMove() * 40.0f;
        if (lobbyScrollY < 0)
            lobbyScrollY = 0;
        if (lobbyScrollY > maxScroll)
            lobbyScrollY = maxScroll;
    }

    BeginScissorMode((int)view.x, (int)view.y, (int)view.width - 18, (int)view.height);

    float y = view.y - lobbyScrollY + 4.0f;

    for (int i = 0; i < lobbyCount; i++)
    {
        Rectangle btnRect = {
            view.x + 8.0f,
            y,
            view.width - 32.0f,
            btnH};

        if (GuiButton(btnRect, lobbies[i].lobbyName))
        {
            gameState = GAME;
        }

        y += btnH + spacing;
    }

    EndScissorMode();

    if (GuiButton((Rectangle){panelX, GAME_HEIGHT - 200, panelWidth, BUTTON_HEIGHT}, "Host Lobby"))
    {
        gameState = LOBBY_CREATOR;
    }
}

void RenderLobbyCreatorGUI()
{

    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH / 2 - panelWidth / 2;

    int titleY = 150;

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiLabel((Rectangle){panelX, titleY, panelWidth, 40}, "Create Lobby");

    if (GuiButton((Rectangle){GAME_WIDTH - 350, 50, 300, BUTTON_HEIGHT}, "Back"))
    {
        gameState = MENU;
    }

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel((Rectangle){panelX, titleY + 200, panelWidth, 30}, "Lobby Name:");

    GuiTextBox((Rectangle){panelX, titleY + 320, panelWidth, BUTTON_HEIGHT}, lobbyName, sizeof(lobbyName), true);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);


    if (GuiButton((Rectangle){panelX, titleY + 480, panelWidth, BUTTON_HEIGHT}, "Create"))
    {
        gameState = WAITING;
    }
}

void RenderWaitingGUI()
{

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    if (GuiButton((Rectangle){GAME_WIDTH - 350, 50, 300, BUTTON_HEIGHT}, "Back"))
    {
        gameState = MENU;
    }

    GuiLabel((Rectangle){0, GAME_HEIGHT / 2, GAME_WIDTH, 30}, "Waiting for players...");
}

void RenderCreditsGUI()
{
}

void UpdateInGameGUI()
{
    UpdateCountdown();
    UpdateMovementTimer();
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

    int minutes = (int)(movementTimer / 60.0f);
    int seconds = (int)(movementTimer) % 60;
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);

    int fontSize = 32;
    DrawText(buffer, 20, 20, fontSize, WHITE);
}
