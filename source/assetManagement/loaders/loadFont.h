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

        uint32_t panelW         = Settings::Font::Default::panelW;
        uint32_t panelH         = Settings::Font::Default::panelH;
        float baseLineFromTop   = Settings::Font::Default::baseLineFromTop;
        float stretchMultW      = Settings::Font::Default::stretchMultW;
        float stretchMultH      = Settings::Font::Default::stretchMultH;
        char firstChar          = Settings::Font::Default::firstChar;
        char lastChar           = Settings::Font::Default::lastChar;

        int getBaseLineFromTopPix();
        float getPanelAdvance();
        uint32_t getNumTexPanels();
        uint32_t getNumTexBytes();
        uint32_t getFTpixelSizeW();
        uint32_t getFTpixelSizeH();
        uint32_t getAtlasWidth();
        uint32_t getAtlasHeight();
        uint32_t getNumPanelPix();
        uint32_t getOffsetToFirstChar();
    };

    bool loadFont(FontDescriptor& font, const char* fontFile, std::stringstream& output);

}

#endif //DUALITYENGINE_LOADFONT_H
