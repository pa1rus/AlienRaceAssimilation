#include <uuid/uuid.h>
#include <stdint.h>
#include <stdlib.h> // For malloc and free
#include <string.h> // For strcmp

#include "gui.h"
#include "hermes.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// --- Constants and Global Variables ---

// Define the maximum number of rooms supported (10 rooms * 16 bytes/room = 160 bytes)
#define MAX_ROOM_CAPACITY 10
#define NAME_SIZE 16
#define UUID_SIZE 16
#define ALLOCATION_SIZE (MAX_ROOM_CAPACITY * NAME_SIZE) // 160 bytes

float lobbyScrollY = 0.0f;
// The lobbies array was removed as it was unnecessary and misused
uint8_t lobbyCount = 0; // Fixed type and initialization

const int BUTTON_HEIGHT = 100;
int defaultFontSize = 64;

// ... countdown and game state variables ...

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

// These must be char* to be assigned the result of malloc
char* lobbyIds = NULL;
char* lobbyNames = NULL;

// --- Function Implementations ---

void InitGUI()
{
    // ... RayGui style setup (no change) ...
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

void UnloadGUI()
{
    // Important: Free memory when the buffers are no longer needed
    if (lobbyIds != NULL) {
        free(lobbyIds);
        lobbyIds = NULL;
    }
    if (lobbyNames != NULL) {
        free(lobbyNames);
        lobbyNames = NULL;
    }
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
        hermesListRooms();
        
        // **FIXED ALLOCATION:** Ensure buffers are allocated or reallocated (safer approach)
        if (lobbyIds == NULL) {
            lobbyIds = malloc(ALLOCATION_SIZE); // 160 bytes for 10 UUIDs
        }
        if (lobbyNames == NULL) {
            lobbyNames = malloc(ALLOCATION_SIZE); // 160 bytes for 10 names
        }

        if (lobbyIds == NULL || lobbyNames == NULL)
        {
            printf("Failed to allocate enough space for lobby buffers\n");
            exit(-3);
        }

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
        UnloadGUI(); // Added memory cleanup
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
        // **CRITICAL FIX:** Use pointer arithmetic to get the start of the i-th 16-byte name.
        char* current_name = lobbyNames + (i * NAME_SIZE); 
        
        // The check for empty names must check the actual data, not a literal string of spaces
        // If the server zeroes out the unused room entries, checking for an empty string might be sufficient.
        if (current_name[0] == '\0') {
             continue;
        }

        Rectangle btnRect = {
            view.x + 8.0f,
            y,
            view.width - 32.0f,
            btnH};

        // Use the calculated pointer to the i-th name
        if (GuiButton(btnRect, current_name))
        {
            gameState = GAME;
            // Now you can access the i-th UUID as well:
            // uuid_t* selected_id = (uuid_t*)(lobbyIds + (i * UUID_SIZE));
            // hermesJoinRoom(my_client_id, selected_id);
        }

        y += btnH + spacing;
    }

    EndScissorMode();

    if (GuiButton((Rectangle){panelX, GAME_HEIGHT - 200, panelWidth, BUTTON_HEIGHT}, "Host Lobby"))
    {
        gameState = LOBBY_CREATOR;
    }
}

// ... other GUI functions (RenderLobbyCreatorGUI, RenderWaitingGUI, etc.) ...
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
