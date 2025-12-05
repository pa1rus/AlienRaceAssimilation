#include "gui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const int BUTTON_HEIGHT = 100;

int defaultFontSize = 64;

void InitGUI()
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 16);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x5555FFFF);   // normal
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x7777FFFF);  // hover
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x3333FFFF);  // pressed
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);   // text color 
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);   // text color 
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xFFFFFFFF);   // text color 
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 4);


}

void RenderMenuGUI()
{
    int panelWidth = GAME_WIDTH / 3;
    int panelX = GAME_WIDTH/2 - panelWidth/2;

    int y = 250;
    int spacing = 50; 

    GuiSetStyle(DEFAULT, TEXT_SIZE, 128);

    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 30 }, "Alien Race");
    y += spacing * 2;

    GuiSetStyle(DEFAULT, TEXT_SIZE, defaultFontSize);

    GuiLabel((Rectangle){ 0, y, GAME_WIDTH, 20 }, "Assimilation");
    y += spacing * 2;

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
