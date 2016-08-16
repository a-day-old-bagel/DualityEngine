//
// Created by volundr on 7/12/16.
//

#include <algorithm>
#include <vector>
#include <math.h>
#include "loadFont.h"
#include "loadShaders.h"
#include "errorChecks.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define CHECK_ERROR(context) glError = glGetError(); if (glError != GL_NO_ERROR) { fprintf(stderr, context " 0x%04x\n", glError); }

namespace DualityEngine {
    int FontDescriptor::getBaseLineFromTopPix() {
        return (int)(panelH * baseLineFromTop);
    }
    uint32_t FontDescriptor::getNumPanels() {
        // +1 for the 'unknown char' character and +1 for including first and last in count
        return (uint32_t)(lastChar - firstChar) + 2;
    }
    uint32_t FontDescriptor::getNumPixTotal() {
        return getNumPanels() * getNumPanelPix();
    }
    uint32_t FontDescriptor::getFTpixelSizeW() {
        return (uint32_t)(panelW * stretchMultW);
    }
    uint32_t FontDescriptor::getFTpixelSizeH() {
        return (uint32_t)(panelH * stretchMultH);
    }
    uint32_t FontDescriptor::getAtlasWidth() {
        return panelW * getNumPanels();
    }
    uint32_t FontDescriptor::getAtlasHeight() {
        return panelH;
    }
    uint32_t FontDescriptor::getNumPanelPix() {
        return panelW * panelH;
    }
    float FontDescriptor::getPanelAdvance() {
        return 1.f / getNumPanels();
    }
    uint32_t FontDescriptor::getOffsetToFirstChar() {
        return 1;
    }
    uint32_t FontDescriptor::getNumPixTotalSdf() {
        return getNumPanelPixSdf() * getNumPanels();
    }
    uint32_t FontDescriptor::getNumPanelPixSdf() {
        return getPanelWSdf() * getPanelHSdf();
    }
    uint32_t FontDescriptor::getPanelWSdf() {
        return panelW + 2 * getFuzzRadiusPixX();
    }
    uint32_t FontDescriptor::getPanelHSdf() {
        return panelH + 2 * getFuzzRadiusPixY();
    }
    uint32_t FontDescriptor::getFuzzRadiusPixX() {
        return (uint32_t)ceil(sdfData.fuzzRadiusRatio * panelW);
    }
    uint32_t FontDescriptor::getFuzzRadiusPixY() {
        return (uint32_t)ceil(sdfData.fuzzRadiusRatio * panelH);
    }
    uint32_t FontDescriptor::getAtlasWidthSdf() {
        return getPanelWSdf() * getNumPanels();
    }
    uint32_t FontDescriptor::getAtlasHeightSdf() {
        return getPanelHSdf();
    }

    static bool loadFromFreetype(FT_Library& library, FT_Face& face,
                                 FontDescriptor& font, const char* fontFile, std::stringstream& output) {
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
        return true;
    }

#define LOAD_UNKNOWN_CHAR_INSTEAD \
    output << "\nERROR loading font character " << i << std::endl;                                          \
    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - font.firstChar + 1) * font.panelW, 0, font.panelW, font.panelH,  \
    GL_RED, GL_FLOAT, unknownCharPanel.data());                                                     \
    continue

#define LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT \
    output << "\nERROR loading font character " << i << std::endl;                                          \
    glTexSubImage2D(GL_TEXTURE_2D, 0, (i - font.firstChar + 1) * font.panelW, 0, font.panelW, font.panelH,  \
    GL_RED, GL_FLOAT, unknownCharPanel.data());                                                     \
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

        std::vector<uint8_t> zeroedTexData(font.getNumPixTotal(), 0);

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
        std::vector<uint8_t> unknownCharPanel(font.getNumPixTotal(), 0);
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
            if (i == ' ') { continue; }

            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);
            error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
            if (error) { LOAD_UNKNOWN_CHAR_INSTEAD; }
            FT_Bitmap bmp = face->glyph->bitmap;

            int bmpW = bmp.width;
            int bmpH = bmp.rows;

            if (bmp.buffer == NULL) {
                LOAD_UNKNOWN_CHAR_INSTEAD;
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
        if (!loadFromFreetype(library, face, font, fontFile, output)) {
            return false;
        }
        FT_Error ftErr;

        std::vector<float> zeroedTexData(font.getNumPixTotalSdf() * sizeof(float), 0);
        uint32_t width = font.getAtlasWidthSdf();
        uint32_t height = font.getAtlasHeightSdf();

        glGenTextures(1, &font.texture);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT,
                     zeroedTexData.data());

        checkError(output, "loadFont.cpp", __LINE__);

        // generate the 'unknown character' block at the front of the font atlas (position 0)
        std::vector<float> unknownCharPanel(font.getNumPixTotal(), 0);
        uint32_t ucpMarginSizeW = (uint32_t)(font.panelW * 0.2f);
        uint32_t ucpMarginSizeH = (uint32_t)(font.panelH * 0.2f);
        for (uint32_t i = ucpMarginSizeH; i < font.panelH - ucpMarginSizeH; ++i) {
            for (uint32_t j = ucpMarginSizeW; j < font.panelW - ucpMarginSizeW; ++j) {
                unknownCharPanel[i * font.panelW + j] = 1.f;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, font.getFuzzRadiusPixX(), font.getFuzzRadiusPixY(), font.panelW, font.panelH,
                        GL_RED, GL_FLOAT, unknownCharPanel.data());

//#define USE_SMOOTH_FINE

#ifndef USE_SMOOTH_FINE
        for (char i = font.firstChar; i <= font.lastChar; i++) {
            if (i == ' ') { continue; }

            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);
            ftErr = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
            if (ftErr) { LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT; }
            FT_Bitmap bmpBits = face->glyph->bitmap;

            int bmpWPadded = bmpBits.pitch * 8;
            int bmpW = bmpBits.width;
            int bmpH = bmpBits.rows;

            if (bmpBits.buffer == NULL) { LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT; }

            // Fill the bytesFromBits vector with one 0xFF byte for each 0x1 bit in the monochrome bitmap
            std::vector<float> bytesFromBits;
            for (size_t j = 0; j < (size_t)(bmpWPadded * bmpH); ++j) {
                if ((int)(j % bmpWPadded) >= bmpW) { continue; }
                bytesFromBits.push_back((uint8_t)((bmpBits.buffer[j / 8] & (1 << (7 - (j % 8)))) == 0 ? 0.f : 1.f));
            }
            // units in 1/64 pixel
            FT_Pos yOffset = font.getBaseLineFromTopPix() - face->glyph->metrics.horiBearingY / 64 +
                    font.getFuzzRadiusPixY();
            FT_Pos xOffset = face->glyph->metrics.horiBearingX / 64 + font.getFuzzRadiusPixX();
            FT_Pos xPanelStart = (i - font.firstChar + 1) * font.getPanelWSdf() + xOffset;
            glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW, bmpH,
                            GL_RED, GL_FLOAT, bytesFromBits.data());

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << std::endl << gluErrorString(glErr) << ": ";
                LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT;
            }
        }
#else
        for (char i = font.firstChar; i <= font.lastChar; i++) {
            if (i == ' ') { continue; }

            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);
            ftErr = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
            if (ftErr) { LOAD_UNKNOWN_CHAR_INSTEAD; }
            FT_Bitmap bmp = face->glyph->bitmap;

            int bmpW = bmp.width;
            int bmpH = bmp.rows;

            if (bmp.buffer == NULL) { LOAD_UNKNOWN_CHAR_INSTEAD; }

            std::vector<float> bytesFromBits;
            for (size_t j = 0; j < (size_t)(bmpW * bmpH); ++j) {
                bytesFromBits.push_back((bmp.buffer[j] / 255.f));
            }
            // units in 1/64 pixel
            FT_Pos yOffset = font.getBaseLineFromTopPix() - face->glyph->metrics.horiBearingY / 64 +
                             font.getFuzzRadiusPixY();
            FT_Pos xOffset = face->glyph->metrics.horiBearingX / 64;
            FT_Pos xPanelStart = (i - font.firstChar + 1) * font.getPanelWSdf() + xOffset + font.getFuzzRadiusPixX();
            glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW, bmpH,
                            GL_RED, GL_FLOAT, bytesFromBits.data());

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << gluErrorString(glErr) << ": " << std::endl;
                LOAD_UNKNOWN_CHAR_INSTEAD;
            }
        }
#endif
        FT_Done_Face(face);
        FT_Done_FreeType(library);



        GLenum glError;
        //TODO: make next four paragraphs into a screenCapture class (which shall also be used in ScreenShot)...
        struct {
            int x, y, w, h;
        } savedViewPort;
        GLuint fbo, colorBuffer, depthBuffer;
        // Bind the screen shot frame buffer for to do color and depth buffer creation
        glGenFramebuffers(1, &fbo);
        CHECK_ERROR("place 1");

        // Create the color component of the framebuffer as a texture
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        CHECK_ERROR("place 2");

        // Create the depth component of the framebuffer as a render buffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "Screen shot framebuffer creation failed: 0x%04x\n", status);
            return 1;
        }
        CHECK_ERROR("place 3");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glGetIntegerv(GL_VIEWPORT, (GLint*)&savedViewPort);
        glViewport(0, 0, width, height);
        CHECK_ERROR("place 4");

        CHECK_ERROR("place -1");
        GLuint vaoLoc;
        glGenVertexArrays(1, &vaoLoc);
        glBindVertexArray(vaoLoc);
        GLuint shdrLoc = loadShaders("genSdfFontTexture.vert", "genSdfFontTexture.frag", output);
        GLint  unifLoc_pxAdvX = glGetUniformLocation(shdrLoc, "pxAdvX");
        GLint  unifLoc_pxAdvY = glGetUniformLocation(shdrLoc, "pxAdvY");
        GLint  unifLoc_fzzRad = glGetUniformLocation(shdrLoc, "fuzzRadius");
        GLint  attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
        GLint  attrLoc_uvCoo = glGetAttribLocation(shdrLoc, "UV");
        GLint  unifLoc_txtre = glGetUniformLocation(shdrLoc, "tex");
        GLint  unifLoc_mvpMx = glGetUniformLocation(shdrLoc, "MVP");
        GLuint buffers[2];
        glGenBuffers(2, buffers);
        CHECK_ERROR("place 0a");
        std::vector<float> verts = {
                -1.f, -1.f, 0.5f,
                 1.f, -1.f, 0.5f,
                -1.f,  1.f, 0.5f,
                -1.f,  1.f, 0.5f,
                 1.f, -1.f, 0.5f,
                 1.f,  1.f, 0.5f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_verts);
        glVertexAttribPointer((GLuint)attrLoc_verts, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_ERROR("place 0b");
        std::vector<float> uvs = {
                0.f, 0.f,
                1.f, 0.f,
                0.f, 1.f,
                0.f, 1.f,
                1.f, 0.f,
                1.f, 1.f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_uvCoo);
        glVertexAttribPointer((GLuint)attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_ERROR("place 0c");

        glUseProgram(shdrLoc);
        glUniform1i(unifLoc_txtre, 1);
        glUniform1f(unifLoc_pxAdvX, 1.f / width);
        glUniform1f(unifLoc_pxAdvY, 1.f / height);
        glUniform1i(unifLoc_fzzRad, (int) std::max(font.getFuzzRadiusPixY(), font.getFuzzRadiusPixX()));
        glUniformMatrix4fv(unifLoc_mvpMx, 1, GL_FALSE, &Constants::duaIdentMat4[0][0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        CHECK_ERROR("place 5");

        uint8_t* pixBuf = new uint8_t[width * height * 4];
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixBuf);
        glViewport(savedViewPort.x, savedViewPort.y, savedViewPort.w, savedViewPort.h);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_ERROR("place 6");

//        uint8_t* tempRow = new uint8_t[width * 4];
//        for (uint32_t i = 0; i < height / 2; ++i) {
//            memcpy(tempRow, pixBuf + i * width * 4, width * 4);
//            memcpy(pixBuf + i * width * 4, pixBuf + (width * (height - 1) * 4 - i * width * 4), width * 4);
//            memcpy(pixBuf + (width * (height - 1) * 4 - i * width * 4), tempRow, width * 4);
//        }
        stbi_write_png("sdfAtlas.png", width, height, 4, pixBuf, 0);
        delete pixBuf;

        //UPLOAD pixBuf

        return true;
    }




    /*bool loadSDFfont(FontDescriptor& font, const char* fontFile, std::stringstream& output) {
        FT_Library library;
        FT_Face face;
        if (!loadFromFreetype(library, face, font, fontFile, output)) {
            return false;
        }
        FT_Error ftErr;

        std::vector<float> zeroedTexData(font.getNumPixTotalSdf() * sizeof(float), 0);
        uint32_t width = font.getAtlasWidthSdf();
        uint32_t height = font.getAtlasHeightSdf();

        glGenTextures(1, &font.texture);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT,
                     zeroedTexData.data());

        checkError(output, "loadFont.cpp", __LINE__);

        // generate the 'unknown character' block at the front of the font atlas (position 0)
        std::vector<float> unknownCharPanel(font.getNumPixTotal(), 0);
        uint32_t ucpMarginSizeW = (uint32_t)(font.panelW * 0.2f);
        uint32_t ucpMarginSizeH = (uint32_t)(font.panelH * 0.2f);
        for (uint32_t i = ucpMarginSizeH; i < font.panelH - ucpMarginSizeH; ++i) {
            for (uint32_t j = ucpMarginSizeW; j < font.panelW - ucpMarginSizeW; ++j) {
                unknownCharPanel[i * font.panelW + j] = 1.f;
            }
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, font.getFuzzRadiusPixX(), font.getFuzzRadiusPixY(), font.panelW, font.panelH,
                        GL_RED, GL_FLOAT, unknownCharPanel.data());

//#define USE_SMOOTH_FINE

#ifndef USE_SMOOTH_FINE
        for (char i = font.firstChar; i <= font.lastChar; i++) {
            if (i == ' ') { continue; }

            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);
            ftErr = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
            if (ftErr) { LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT; }
            FT_Bitmap bmpBits = face->glyph->bitmap;

            int bmpWPadded = bmpBits.pitch * 8;
            int bmpW = bmpBits.width;
            int bmpH = bmpBits.rows;

            if (bmpBits.buffer == NULL) { LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT; }

            // Fill the bytesFromBits vector with one 0xFF byte for each 0x1 bit in the monochrome bitmap
            std::vector<float> bytesFromBits;
            for (size_t j = 0; j < (size_t)(bmpWPadded * bmpH); ++j) {
                if ((int)(j % bmpWPadded) >= bmpW) { continue; }
                bytesFromBits.push_back((uint8_t)((bmpBits.buffer[j / 8] & (1 << (7 - (j % 8)))) == 0 ? 0.f : 1.f));
            }
            // units in 1/64 pixel
            FT_Pos yOffset = font.getBaseLineFromTopPix() - face->glyph->metrics.horiBearingY / 64 +
                             font.getFuzzRadiusPixY();
            FT_Pos xOffset = face->glyph->metrics.horiBearingX / 64 + font.getFuzzRadiusPixX();
            FT_Pos xPanelStart = (i - font.firstChar + 1) * font.getPanelWSdf() + xOffset;
            glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW, bmpH,
                            GL_RED, GL_FLOAT, bytesFromBits.data());

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << std::endl << gluErrorString(glErr) << ": ";
                LOAD_UNKNOWN_CHAR_INSTEAD_FLOAT;
            }
        }
#else
        for (char i = font.firstChar; i <= font.lastChar; i++) {
            if (i == ' ') { continue; }

            FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong) i);
            ftErr = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
            if (ftErr) { LOAD_UNKNOWN_CHAR_INSTEAD; }
            FT_Bitmap bmp = face->glyph->bitmap;

            int bmpW = bmp.width;
            int bmpH = bmp.rows;

            if (bmp.buffer == NULL) { LOAD_UNKNOWN_CHAR_INSTEAD; }

            std::vector<float> bytesFromBits;
            for (size_t j = 0; j < (size_t)(bmpW * bmpH); ++j) {
                bytesFromBits.push_back((bmp.buffer[j] / 255.f));
            }
            // units in 1/64 pixel
            FT_Pos yOffset = font.getBaseLineFromTopPix() - face->glyph->metrics.horiBearingY / 64 +
                             font.getFuzzRadiusPixY();
            FT_Pos xOffset = face->glyph->metrics.horiBearingX / 64;
            FT_Pos xPanelStart = (i - font.firstChar + 1) * font.getPanelWSdf() + xOffset + font.getFuzzRadiusPixX();
            glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xPanelStart, (GLint)std::max(yOffset, (FT_Pos)0), bmpW, bmpH,
                            GL_RED, GL_FLOAT, bytesFromBits.data());

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR) {
                output << gluErrorString(glErr) << ": " << std::endl;
                LOAD_UNKNOWN_CHAR_INSTEAD;
            }
        }
#endif
        FT_Done_Face(face);
        FT_Done_FreeType(library);



        GLenum glError;
        //TODO: make next four paragraphs into a screenCapture class (which shall also be used in ScreenShot)...
        struct {
            int x, y, w, h;
        } savedViewPort;
        GLuint fbo, colorBuffer, depthBuffer;
        // Bind the screen shot frame buffer for to do color and depth buffer creation
        glGenFramebuffers(1, &fbo);
        CHECK_ERROR("place 1");

        // Create the color component of the framebuffer as a texture
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        CHECK_ERROR("place 2");

        // Create the depth component of the framebuffer as a render buffer
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "Screen shot framebuffer creation failed: 0x%04x\n", status);
            return 1;
        }
        CHECK_ERROR("place 3");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glGetIntegerv(GL_VIEWPORT, (GLint*)&savedViewPort);
        glViewport(0, 0, width, height);
        CHECK_ERROR("place 4");

        CHECK_ERROR("place -1");
        GLuint vaoLoc;
        glGenVertexArrays(1, &vaoLoc);
        glBindVertexArray(vaoLoc);
        GLuint shdrLoc = loadShaders("genSdfFontTexture.vert", "genSdfFontTexture.frag", output);
        GLint  unifLoc_pxAdvX = glGetUniformLocation(shdrLoc, "pxAdvX");
        GLint  unifLoc_pxAdvY = glGetUniformLocation(shdrLoc, "pxAdvY");
        GLint  unifLoc_fzzRad = glGetUniformLocation(shdrLoc, "fuzzRadius");
        GLint  attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
        GLint  attrLoc_uvCoo = glGetAttribLocation(shdrLoc, "UV");
        GLint  unifLoc_txtre = glGetUniformLocation(shdrLoc, "tex");
        GLint  unifLoc_mvpMx = glGetUniformLocation(shdrLoc, "MVP");
        GLuint buffers[2];
        glGenBuffers(2, buffers);
        CHECK_ERROR("place 0a");
        std::vector<float> verts = {
                -1.f, -1.f, 0.5f,
                1.f, -1.f, 0.5f,
                -1.f,  1.f, 0.5f,
                -1.f,  1.f, 0.5f,
                1.f, -1.f, 0.5f,
                1.f,  1.f, 0.5f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_verts);
        glVertexAttribPointer((GLuint)attrLoc_verts, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_ERROR("place 0b");
        std::vector<float> uvs = {
                0.f, 0.f,
                1.f, 0.f,
                0.f, 1.f,
                0.f, 1.f,
                1.f, 0.f,
                1.f, 1.f,
        };
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_uvCoo);
        glVertexAttribPointer((GLuint)attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_ERROR("place 0c");

        glUseProgram(shdrLoc);
        glUniform1i(unifLoc_txtre, 1);
        glUniform1f(unifLoc_pxAdvX, 1.f / width);
        glUniform1f(unifLoc_pxAdvY, 1.f / height);
        glUniform1i(unifLoc_fzzRad, (int) std::max(font.getFuzzRadiusPixY(), font.getFuzzRadiusPixX()));
        glUniformMatrix4fv(unifLoc_mvpMx, 1, GL_FALSE, &Constants::duaIdentMat4[0][0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, font.texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        CHECK_ERROR("place 5");

        uint8_t* pixBuf = new uint8_t[width * height * 4];
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixBuf);
        glViewport(savedViewPort.x, savedViewPort.y, savedViewPort.w, savedViewPort.h);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_ERROR("place 6");

//        uint8_t* tempRow = new uint8_t[width * 4];
//        for (uint32_t i = 0; i < height / 2; ++i) {
//            memcpy(tempRow, pixBuf + i * width * 4, width * 4);
//            memcpy(pixBuf + i * width * 4, pixBuf + (width * (height - 1) * 4 - i * width * 4), width * 4);
//            memcpy(pixBuf + (width * (height - 1) * 4 - i * width * 4), tempRow, width * 4);
//        }
        stbi_write_png("sdfAtlas.png", width, height, 4, pixBuf, 0);
        delete pixBuf;

        //UPLOAD pixBuf

        return true;
    }*/



#undef LOAD_UNKNOWN_CHAR_INSTEAD
}