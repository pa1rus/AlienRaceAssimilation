#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdint.h>
#include <stdio.h>

const int BUTTON_HEIGHT = 100;
int defaultFontSize = 64;

uint32_t HexToABGR(uint32_t hex, uint8_t alpha) {
    uint32_t r = (hex >> 16) & 0xFF;
    uint32_t g = (hex >> 8) & 0xFF;
    uint32_t b = hex & 0xFF;
    uint32_t a = alpha;

    return (a << 24) | (b << 16) | (g << 8) | r;
}


void InitGUI()
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, HexToABGR(0xF6D6BD, 0xFF));   // #F6D6BD
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 16);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, HexToABGR(0x977577, 0xFF));   // #977577
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, HexToABGR(0x816271, 0xFF));  // #816271
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, HexToABGR(0x4E495F, 0xFF));  // #4E495F
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, HexToABGR(0xF6D6BD, 0xFF));   // #F6D6BD
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, HexToABGR(0xF6D6BD, 0xFF));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, HexToABGR(0xF6D6BD, 0xFF));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, HexToABGR(0xF6D6BD, 0xFF));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, HexToABGR(0xF6D6BD, 0xFF));
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, HexToABGR(0xF6D6BD, 0xFF));
    GuiSetStyle(BUTTON, BORDER_WIDTH, 4);
}

void RenderMenuGUI()
{
    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH/2 - panelWidth/2;

    int y = 250;
    int spacing = 50; 

    GuiSetStyle(DEFAULT, TEXT_SIZE, 128);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, HexToABGR(0xF6D6BD, 0xFF));
    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 30 }, "Alien Race");
    y += spacing * 2;

    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, HexToABGR(0xC3A38A, 0xFF));
    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 20 }, "Assimilation");
    y += spacing * 3;

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, HexToABGR(0xF6D6BD, 0xFF));

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Play")) {}
    y += BUTTON_HEIGHT + spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Rules")) {}
    y += BUTTON_HEIGHT + spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Exit")) {}
}

void RenderLobbySelectorGUI()
{
    int panelWidth = 300;
    int panelX = GAME_WIDTH/2 - panelWidth/2;

    int y = 150;
    int spacing = 50;

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, HexToABGR(0xF6D6BD, 0xFF));
    GuiLabel((Rectangle){ panelX, y, panelWidth, 30 }, "Select Lobby");
    y += spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, 30 }, "Lobby 1")) {}
}

void RenderInGameGUI() {}
