//
// Created by volundr on 7/12/16.
//

#include <algorithm>
#include <vector>
#include "loadFont.h"
#include "errorChecks.h"

namespace DualityEngine {

    int FontDescriptor::getBaseLineFromTopPix() {
        return (int)(panelH * baseLineFromTop);
    }

    uint32_t FontDescriptor::getNumTexPanels() {
        // +1 for the 'unknown char' character and +1 for including first and last in count
        return (uint32_t)(lastChar - firstChar) + 2;
    }

    uint32_t FontDescriptor::getNumTexBytes() {
        return getNumTexPanels() * panelW * panelH;
    }

    uint32_t FontDescriptor::getFTpixelSizeW() {
        return (uint32_t)(panelW * stretchMultW);
    }

    uint32_t FontDescriptor::getFTpixelSizeH() {
        return (uint32_t)(panelH * stretchMultH);
    }

    uint32_t FontDescriptor::getAtlasWidth() {
        return panelW * getNumTexPanels();
    }

    uint32_t FontDescriptor::getAtlasHeight() {
        return panelH;
    }

    uint32_t FontDescriptor::getNumPanelPix() {
        return panelW * panelH;
    }

    float FontDescriptor::getPanelAdvance() {
        return 1.f / getNumTexPanels();
    }

    uint32_t FontDescriptor::getOffsetToFirstChar() {
        return 1;
    }


    static bool uploadTexture() {
        return false;
    }

    static bool uploadUnknownChar() {
        return true;
    }

#define LOAD_UNKNOWN_CHAR_INSTEAD \
    output << "\nERROR loading font character " << i << std::endl;                                          \
    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - font.firstChar + 1) * font.panelW, 0, font.panelW, font.panelH,  \
    GL_RED, GL_UNSIGNED_BYTE, unknownCharPanel.data());                                                     \
    continue

    bool loadFont(FontDescriptor& font, const char* fontFile, std::stringstream& output) {
        FT_Library library;
        FT_Face face;

        FT_Error error = FT_Init_FreeType(&library);
        if (error) {
            output << "\nERROR initializing freeType library.\n";
            return false;
        }
        std::string fontFilePath = Settings::Font::assetDirPrefix + std::string(fontFile);
        error = FT_New_Face(library, fontFilePath.c_str(), 0, &face);
        if (error == FT_Err_Unknown_File_Format) {
            output << "\nERROR loading font: unknown file format.\n";
            return false;
        } else if (error) {
            output << "\nERROR loading font.\n";
            return false;
        }

        error = FT_Set_Pixel_Sizes(face, font.getFTpixelSizeW(), font.getFTpixelSizeH());
        if (error) {
            output << "\nERROR setting font size.\n";
            return false;
        }

        std::vector<uint8_t> zeroedTexData(font.getNumTexBytes(), 0);

        glGenTextures(1, &font.texture);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.getAtlasWidth(), font.getAtlasHeight(), 0, GL_RED, GL_UNSIGNED_BYTE,
                     zeroedTexData.data());

        checkError(output, "loadFont.cpp", __LINE__);

        // generate the 'unknown character' block at the front of the font atlas (position 0)
        std::vector<uint8_t> unknownCharPanel(font.getNumTexBytes(), 0);
        uint32_t ucpMarginSizeW = (uint32_t)(font.panelW * 0.2f);
        uint32_t ucpMarginSizeH = (uint32_t)(font.panelH * 0.2f);
        for (uint32_t i = ucpMarginSizeH; i < font.panelH - ucpMarginSizeH; ++i) {
            for (uint32_t j = ucpMarginSizeW; j < font.panelW - ucpMarginSizeW; ++j) {
                unknownCharPanel[i * font.panelW + j] = 0x60;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, font.panelW, font.panelH, GL_RED, GL_UNSIGNED_BYTE,
                        unknownCharPanel.data());

        FT_Pos xOffset = 0;
        FT_Pos yOffset = 0;
        FT_Pos xPanelStart;
        for (char i = font.firstChar; i <= font.lastChar; i++) {

            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);
            error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
            if (error) { LOAD_UNKNOWN_CHAR_INSTEAD; }
            FT_Bitmap bmp = face->glyph->bitmap;

            int bmpW = bmp.width;
            int bmpH = bmp.rows;

            if (bmp.buffer == NULL) {
                if (i != ' ') { LOAD_UNKNOWN_CHAR_INSTEAD; }
            } else {
                // units in 1/64 pixel
                yOffset = font.getBaseLineFromTopPix() - face->glyph->metrics.horiBearingY / 64;
                xOffset = face->glyph->metrics.horiBearingX / 64;
                xPanelStart = (i - font.firstChar + 1) * font.panelW + xOffset;
                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW,
                                (GLint)std::min({(FT_Pos)bmpH, (FT_Pos)font.panelH - yOffset, (FT_Pos)font.panelH}),
                                GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);
            }

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {

                xPanelStart = (i - font.firstChar + 1) * font.panelW + (font.panelW - bmpW);
                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW,
                                (GLint)std::min({(FT_Pos)bmpH, (FT_Pos)font.panelH - yOffset, (FT_Pos)font.panelH}),
                                GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);

                glErr = glGetError();
                if (glErr != GL_NO_ERROR) {
                    output << gluErrorString(glErr) << ": " << std::endl;
                    LOAD_UNKNOWN_CHAR_INSTEAD;
                }
            }
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        return true;
    }

    bool loadSDFfont(FontDescriptor& font, const char* fontFile, std::stringstream& output) {
        FT_Library library;
        FT_Face face;

        FT_Error error = FT_Init_FreeType(&library);
        if (error) {
            output << "\nERROR initializing freeType library.\n";
            return false;
        }
        std::string fontFilePath = Settings::Font::assetDirPrefix + std::string(fontFile);
        error = FT_New_Face(library, fontFilePath.c_str(), 0, &face);
        if (error == FT_Err_Unknown_File_Format) {
            output << "\nERROR loading font: unknown file format.\n";
            return false;
        } else if (error) {
            output << "\nERROR loading font.\n";
            return false;
        }

        error = FT_Set_Pixel_Sizes(face, font.getFTpixelSizeW(), font.getFTpixelSizeH());
        if (error) {
            output << "\nERROR setting font size.\n";
            return false;
        }

        std::vector<uint8_t> zeroedTexData(font.getNumTexBytes(), 0);

        glGenTextures(1, &font.texture);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.getAtlasWidth(), font.getAtlasHeight(), 0, GL_RED, GL_UNSIGNED_BYTE,
                     zeroedTexData.data());

        checkError(output, "loadFont.cpp", __LINE__);

        // generate the 'unknown character' block at the front of the font atlas (position 0)
        std::vector<uint8_t> unknownCharPanel(font.getNumTexBytes(), 0);
        uint32_t ucpMarginSizeW = (uint32_t)(font.panelW * 0.2f);
        uint32_t ucpMarginSizeH = (uint32_t)(font.panelH * 0.2f);
        for (uint32_t i = ucpMarginSizeH; i < font.panelH - ucpMarginSizeH; ++i) {
            for (uint32_t j = ucpMarginSizeW; j < font.panelW - ucpMarginSizeW; ++j) {
                unknownCharPanel[i * font.panelW + j] = 0x60;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, font.panelW, font.panelH, GL_RED, GL_UNSIGNED_BYTE,
                        unknownCharPanel.data());

        FT_Pos xOffset = 0;
        FT_Pos yOffset = 0;
        FT_Pos xPanelStart;
        FT_Glyph glyph;
        for (char i = font.firstChar; i <= font.lastChar; i++) {
            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);

//            error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
//            if (error) { LOAD_UNKNOWN_CHAR_INSTEAD; }
//            error = FT_Get_Glyph( face->glyph, &glyph );
//            if (error) { LOAD_UNKNOWN_CHAR_INSTEAD; }
//            error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, NULL, 1);
//            if (error) { LOAD_UNKNOWN_CHAR_INSTEAD; }
//            FT_BitmapGlyph bmpGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
//            FT_Bitmap bmp = bmpGlyph->bitmap;

            error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
            if (error) { LOAD_UNKNOWN_CHAR_INSTEAD; }
            FT_Bitmap bmpBits = face->glyph->bitmap;
//            FT_Bitmap bmpBytes;

//            FT_Bitmap_Convert(library, &bmpBits, &bmpBytes, 8);

//            output << i << " " << bmpBytes.pitch << " -> " << bmpBytes.pitch << std::endl;

            int bmpW = bmpBits.width;
            int bmpH = bmpBits.rows;

            std::vector<uint8_t> expandedBitsToBytes((size_t)(bmpW * bmpH), 0);

            if (bmpBits.buffer == NULL) {
                if (i != ' ') { LOAD_UNKNOWN_CHAR_INSTEAD; }
            } else {

                for (size_t j = 0; j < (size_t)(bmpW * bmpH); ++j) {
//                    if ((j % 8) / 8 < bmpBits.pitch) {
                        expandedBitsToBytes[j] = (uint8_t) ((bmpBits.buffer[j / 8] & (1 << (7 - (j % 8)))) == 0 ? 0 : 255);
//                    } else {
//                        expandedBitsToBytes[j] = 127;
//                    }
                }

                // units in 1/64 pixel
                yOffset = font.getBaseLineFromTopPix() - face->glyph->metrics.horiBearingY / 64;
                xOffset = face->glyph->metrics.horiBearingX / 64;
                xPanelStart = (i - font.firstChar + 1) * font.panelW + xOffset;
//                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW,
//                                (GLint)std::min({(FT_Pos)bmpH, (FT_Pos)font.panelH - yOffset, (FT_Pos)font.panelH}),
//                                GL_RED, GL_UNSIGNED_BYTE, expandedBitsToBytes.data());
                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpBits.pitch * 8, bmpH,
                                GL_RED, GL_UNSIGNED_BYTE, expandedBitsToBytes.data());
            }

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {

                xPanelStart = (i - font.firstChar + 1) * font.panelW + (font.panelW - bmpW);
//                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW,
//                                (GLint)std::min({(FT_Pos)bmpH, (FT_Pos)font.panelH - yOffset, (FT_Pos)font.panelH}),
//                                GL_RED, GL_UNSIGNED_BYTE, expandedBitsToBytes.data());
                glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpBits.pitch * 8,
                                (GLint)std::min({(FT_Pos)bmpH, (FT_Pos)font.panelH - yOffset, (FT_Pos)font.panelH}),
                                GL_RED, GL_UNSIGNED_BYTE, expandedBitsToBytes.data());

                glErr = glGetError();
                if (glErr != GL_NO_ERROR) {
                    output << gluErrorString(glErr) << ": " << std::endl;
                    LOAD_UNKNOWN_CHAR_INSTEAD;
                }
            }
        }

//        FT_Done_Glyph(glyph);
        FT_Done_Face(face);
        FT_Done_FreeType(library);

//        delete[] unknownCharPanel;

        return true;
    }

#undef LOAD_UNKNOWN_CHAR_INSTEAD
}