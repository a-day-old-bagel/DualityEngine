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

        TextFieldParams bodyParams;
        bodyParams.initType     = TextFieldParams::FIELD_SIZE_CHAR_SIZE;
        bodyParams.fieldSize    = { 1.0 ,  0.5 };
        bodyParams.charSize     = { 0.01,  0.02};
        bodyParams.spacing      = { 0.1 ,  0.1 };
        bodyParams.repo         = &fontRepo;
        bodyParams.out          = &output;
        bodyParams.fontName     = Settings::Console::fontName.c_str();
        bodyParams.screenResX   = (uint32_t)Settings::Display::screenResX;
        bodyParams.screenResY   = (uint32_t)Settings::Display::screenResY;

        if (!consoleDrawer.init(bodyParams, console)) {
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