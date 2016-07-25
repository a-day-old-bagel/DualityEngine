//
// Created by volundr on 7/12/16.
//

#include <algorithm>
#include "loadFont.h"
#include "errorChecks.h"

namespace DualityEngine {

    int FontDescriptor::getBaseLineFromTopPix() {
        return (int)(panelH * baseLineFromTop);
    }

    uint32_t FontDescriptor::getNumTexPanels() {
        // extra panel for the 'unknown char' character
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
        // extra factor in width is just 'cause fonts were looking "skinny." Will need modification to use other fonts.
        error = FT_Set_Pixel_Sizes(face, font.getFTpixelSizeW(), font.getFTpixelSizeH());
        if (error) {
            output << "\nERROR setting font size.\n";
            return false;
        }

        DUA_uint8* baseTexData = new DUA_uint8[font.getNumTexBytes()];
        memset(baseTexData, 0x00, font.getNumTexBytes());

        glGenTextures(1, &font.texture);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.getAtlasWidth(), font.getAtlasHeight(), 0, GL_RED, GL_UNSIGNED_BYTE,
                     &baseTexData[0]);

        checkError(output, "Render_Console.cpp", __LINE__);

        DUA_colorByte* firstPanel = new DUA_colorByte[font.getNumPanelPix()];
        for (uint32_t i = 2; i < font.panelH - 2; ++i) {
            for (uint32_t j = 2; j < font.panelW - 2; ++j) {
                firstPanel[i * font.panelW + j] = 0xA0;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, font.panelW, font.panelH, GL_RED, GL_UNSIGNED_BYTE, firstPanel);

        FT_Pos xOffset = 0;
        FT_Pos yOffset = 0;
        FT_Pos xPanelStart;
        for (char i = font.firstChar; i <= font.lastChar; i++) {
            FT_UInt glyph_index = FT_Get_Char_Index(face, (int) i);

            error = FT_Load_Glyph(face, glyph_index /*i*/, FT_LOAD_RENDER);
            if (error) {
                output << "\nERROR loading font character " << i << std::endl;
                glTexSubImage2D(GL_TEXTURE_2D, 0, (i - font.firstChar + 1) * font.panelW, 0, font.panelW, font.panelH,
                                GL_RED, GL_UNSIGNED_BYTE, firstPanel);
                continue;
            }

            FT_Bitmap bmp = face->glyph->bitmap;
            int bmpW = bmp.width;
            int bmpH = bmp.rows;

            if (bmp.buffer == NULL) {
                if (i != ' ') {  // because why would there be a bitmap for 'space'?
                    output << "No bitmap loaded for character '" << i << "'.\n";
                    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - font.firstChar + 1) * font.panelW, 0,
                                    font.panelW, font.panelH, GL_RED, GL_UNSIGNED_BYTE, firstPanel);
                    continue;
                }
            } else {
                // units in 1/64 pixel (stupid freetype)
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
                    output << "glError while subbing texture for '" << i << "': " << gluErrorString(glErr) << std::endl;
                    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - font.firstChar + 1) * font.panelW, 0,
                                    font.panelW, font.panelW, GL_RED, GL_UNSIGNED_BYTE, firstPanel);
                    continue;
                }
            }
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        delete[] firstPanel;
        delete[] baseTexData;

        return true;
    }
}