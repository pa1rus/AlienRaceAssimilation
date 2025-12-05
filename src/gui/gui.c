#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdint.h>
#include <stdio.h>

const int BUTTON_HEIGHT = 100;

int defaultFontSize = 64;

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
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xC3A38AFF); // #C3A38A
    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 20 }, "Assimilation");
    y += spacing * 3;

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xF6D6BDFF);

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Play")) {
    }
    y += BUTTON_HEIGHT;
    y += spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Rules")) {
    }
    y += BUTTON_HEIGHT;
    y += spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, BUTTON_HEIGHT  }, "Exit")) {
    }
}

void RenderLobbySelectorGUI()
{
    int panelWidth = 300;
    int panelX = GAME_WIDTH/2 - panelWidth/2;

    int y = 150;
    int spacing = 50;

    GuiLabel((Rectangle){ panelX, y, panelWidth, 30 }, "Select Lobby");
    y += spacing;

    if (GuiButton((Rectangle){ panelX, y, panelWidth, 30 }, "Lobby 1")) {
    }
}

void RenderInGameGUI()
{
}
