/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 *
 ****************************************************************/
#include "Render_UI.h"

namespace DualityEngine {

    System_Render_UI::System_Render_UI(Bank *bank, Console *console)
            : System<System_Render_UI>(bank, "Console Rendering System", 0), console(console) { }

    bool System_Render_UI::onInit(std::stringstream &output) {

        float stdCharW = 0.02f / Settings::Display::screenAspectRatio;
        float stdCharH = 0.04f;

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

        consoleFont = consoleDrawer.getFont();
        consoleFontView.init(&consoleFont, output, 0, 5.5, 0);

        sdfFont.panelW = 16;
        sdfFont.panelH = 32;
        sdfFont.sdf = true;
        fontRepo.request(consoleParams.fontName.c_str(), sdfFont, output);
        sdfView.init(&sdfFont, output, 0, 3, 0);

        return true;
    }
    void System_Render_UI::onTick() {
        sdfView.draw(bank->pFreeCameraCurrent);
        consoleFontView.draw(bank->pFreeCameraCurrent);
        if (console->consoleIsActive || console->menuIsActive) {
            consoleDrawer.draw();
        }
    }
    void System_Render_UI::onClean() {

    }
}