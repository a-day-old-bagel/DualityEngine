//
// Created by volundr on 7/12/16.
//

#ifndef DUALITYENGINE_LOADFONT_H
#define DUALITYENGINE_LOADFONT_H

#include <sstream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H
#include "settings.h"

namespace DualityEngine {

    struct SdfData {
        float fuzzRadiusRatio   = Settings::Font::Sdf::Default::fuzzRadiusRatio;
        uint32_t inputW         = Settings::Font::Sdf::Default::inputW;
        uint32_t inputH         = Settings::Font::Sdf::Default::inputH;
    };

    struct FontDescriptor {

        GLuint texture;

        uint32_t panelW         = Settings::Font::Default::panelW;
        uint32_t panelH         = Settings::Font::Default::panelH;
        float baseLineFromTop   = Settings::Font::Default::baseLineFromTop;
        float stretchMultW      = Settings::Font::Default::stretchMultW;
        float stretchMultH      = Settings::Font::Default::stretchMultH;
        char firstChar          = Settings::Font::Default::firstChar;
        char lastChar           = Settings::Font::Default::lastChar;
        bool sdf                = false;
        SdfData sdfData;

        int getBaseLineFromTopPix();
        float getPanelAdvance();
        uint32_t getNumPanels();
        uint32_t getNumPixTotal();
        uint32_t getFTpixelSizeW();
        uint32_t getFTpixelSizeH();
        uint32_t getAtlasWidth();
        uint32_t getAtlasHeight();
        uint32_t getNumPanelPix();
        uint32_t getOffsetToFirstChar();

        uint32_t getNumPixTotalSdf();
        uint32_t getNumPanelPixSdf();
        uint32_t getPanelWSdf();
        uint32_t getPanelHSdf();
        uint32_t getFuzzRadiusPixX();
        uint32_t getFuzzRadiusPixY();
        uint32_t getAtlasWidthSdf();
        uint32_t getAtlasHeightSdf();
    };

    bool loadFont(FontDescriptor& font, const char* fontFile, std::stringstream& output);
    bool loadSDFfont(FontDescriptor& font, const char* fontFile, std::stringstream& output);

}

#endif //DUALITYENGINE_LOADFONT_H
