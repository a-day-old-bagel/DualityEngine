//
// Created by volundr on 7/12/16.
//

#ifndef DUALITYENGINE_LOADFONT_H
#define DUALITYENGINE_LOADFONT_H

#include <sstream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "settings.h"

namespace DualityEngine {
    struct FontDescriptor {
        GLuint texture;
        uint32_t panelW = Settings::Console::charW;
        uint32_t panelH = Settings::Console::charH;
        float baseLineFromTop = Settings::Console::baseLineFromTop;
        float stretchMultW = 1.f;
        float stretchMultH = 1.f;
        char firstChar = Settings::Console::firstChar;
        char lastChar = Settings::Console::lastChar;

        int getBaseLineFromTopPix();
        float getPanelAdvance();
        uint32_t getNumTexPanels();
        uint32_t getNumTexBytes();
        uint32_t getFTpixelSizeW();
        uint32_t getFTpixelSizeH();
        uint32_t getAtlasWidth();
        uint32_t getAtlasHeight();
        uint32_t getNumPanelPix();
    };
    int loadFont(FontDescriptor& font, const char* fontFile, std::stringstream& output);
}

#endif //DUALITYENGINE_LOADFONT_H
