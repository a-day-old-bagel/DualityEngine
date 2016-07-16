/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 *
 ****************************************************************/
#include "Render_UI.h"
#include "errorChecks.h"

namespace DualityEngine {

    System_Render_UI::System_Render_UI(Bank *bank, Console *console)
            : System<System_Render_UI>(bank, "Console Rendering System", 0), console(console) {
        font.panelW             = Settings::Console::panelW;
        font.panelH             = Settings::Console::panelH;
        font.baseLineFromTop    = Settings::Console::baseLineFromTop;
        font.stretchMultW       = Settings::Console::stretchMultW;
        font.stretchMultH       = Settings::Console::stretchMultH;
        font.firstChar          = Settings::Console::firstChar;
        font.lastChar           = Settings::Console::lastChar;
    }

    bool System_Render_UI::init(std::stringstream &output) {

        float stdCharW = 0.01f / Settings::Display::screenAspectRatio;
        float stdCharH = 0.02f;

        TextFieldParams consoleParams;
        consoleParams.initType      = TextFieldParams::FIELD_SIZE_CHAR_SIZE;
        consoleParams.fieldSize     = {  1.0f   ,   0.5f   };   // with relation to window width/height
        consoleParams.charSize      = { stdCharW, stdCharH };   // with relation to window width/height
        consoleParams.spacing       = {  0.1f   ,   0.1f   };   // with relation to CHARACTER width/height
        consoleParams.repo          = &fontRepo;
        consoleParams.out           = &output;
        consoleParams.fontName      = Settings::Console::fontName.c_str();
        consoleParams.screenResX    = (uint32_t)Settings::Display::windowResX;
        consoleParams.screenResY    = (uint32_t)Settings::Display::windowResY;

        if (!consoleDrawer.init(consoleParams, console)) {
            return false;
        }
        consoleDrawer.setPosition(0.f, 0.5f);

        return true;
    }
    void System_Render_UI::tick() {
        if (console->consoleIsActive || console->menuIsActive) {
            consoleDrawer.draw();
        }
    }
}