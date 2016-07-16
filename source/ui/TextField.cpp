//
// Created by volundr on 7/11/16.
//

#include <vector>
#include <limits>
#include "TextField.h"
#include "loadShaders.h"
#include "errorChecks.h"

namespace DualityEngine {

    float TextFieldParams::getAdvX() {
        return charSize.w + charSize.w * spacing.x;
    }

    float TextFieldParams::getAdvY() {
        return charSize.h + charSize.h * spacing.y;
    }

    bool TextField::init(TextFieldParams& params) {

        // These metrics will be used to generate geometry
        float fieldW, fieldH;
        float bodyW, bodyH;
        float marginW, marginH;
        uint_fast32_t numCharsX, numCharsY;

        // Depending on whether the user provided field size or number of characters, define the variables above.
        switch(params.initType) {
            case TextFieldParams::FIELD_SIZE_CHAR_SIZE:
                fieldW = params.fieldSize.w;
                fieldH = params.fieldSize.h;
                numCharsX = (uint32_t)(fieldW / params.getAdvX());
                numCharsY = (uint32_t)(fieldH / params.getAdvY());
                bodyW = params.charSize.w + (params.getAdvX() * (numCharsX - 1));
                bodyH = params.charSize.h + (params.getAdvY() * (numCharsY - 1));
                marginW = 0.5f * (fieldW - bodyW);
                marginH = 0.5f * (fieldH - bodyH);
                break;
            case TextFieldParams::CHAR_NUM_CHAR_SIZE:
                numCharsX = params.numChars.x;
                numCharsY = params.numChars.y;
                fieldW = params.charSize.w + (params.getAdvX() * (numCharsX - 1));
                fieldH = params.charSize.h + (params.getAdvY() * (numCharsY - 1));
                bodyW = fieldW;
                bodyH = fieldH;
                marginW = 0;
                marginH = 0;
                break;
            case TextFieldParams::INVALID:
            default:
                *params.out << "<!>    TextFieldParam initType invalid!\n";
                return false;
        }

        // Calculate values that will be used below
        uint_fast32_t numPanelVerts  = numCharsX * numCharsY * 4;
        uint_fast32_t numPanelTris   = numPanelVerts / 2;
        uint_fast32_t sizeVertArray  = numPanelVerts * 2;
        uint_fast32_t sizeIndxArray  = numPanelTris  * 3;

        // Allocate temporary vectors to hold the vertex, index, and uv data as it's generated
        std::vector<float> verts(sizeVertArray);
        std::vector<float> uvs  (sizeVertArray, 0.f);
        std::vector<uint32_t> indices(sizeIndxArray);

        // Generate all of the vertex and index data, leaving the uv data as zeros for now
        for (uint_fast32_t y = 0; y < numCharsY; ++y) {
            for (uint_fast32_t x = 0; x < numCharsX; ++x) {
                uint_fast32_t vStride = (y * numCharsX * 8) + (x * 8);
                uint_fast32_t iStride = (y * numCharsX * 6) + (x * 6);
                for (int i = 0; i < 4; ++i) {
                    verts[vStride +2*i+0] = -bodyW + 2.f * (marginW + params.getAdvX() * x + params.charSize.w * (i%2));
                    verts[vStride +2*i+1] =  bodyH - 2.f * (marginH + params.getAdvY() * y + params.charSize.h * (i/2));
                }
                for (int i = 0; i < 6; ++i) {
                    indices[iStride + i] = (uint32_t)((vStride >> 1) + (i < 3 ? -1: 1) * (-2 + i));
                }
            }
        }

        // Load the shader and pull references to its attributes and uniforms
        shdrLoc = loadShaders("assets/shaders/textField.vert", "assets/shaders/textField.frag", *params.out);
        attrLoc_verts = glGetAttribLocation(shdrLoc, "Vertex");
        attrLoc_uvCoo = glGetAttribLocation(shdrLoc, "UV");
        txtrLoc_chars = glGetUniformLocation(shdrLoc, "font_texture");
        unifLoc_postn = glGetUniformLocation(shdrLoc, "Position");
        unifLoc_color = glGetUniformLocation(shdrLoc, "penColor");

        // Generate the VAO and VBOs
        glGenVertexArrays(1, &vaoLoc);
        glBindVertexArray(vaoLoc);
        glGenBuffers(3, buffers);

        // buffer the vertex data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(float), verts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_verts);
        glVertexAttribPointer((GLuint)attrLoc_verts, 2, GL_FLOAT, GL_FALSE, 0, 0);
        // buffer the zero'd UV data to GPU
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeVertArray * sizeof(float), uvs.data(), GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray((GLuint)attrLoc_uvCoo);
        glVertexAttribPointer((GLuint)attrLoc_uvCoo, 2, GL_FLOAT, GL_FALSE, 0, 0);
        // buffer the index data to GPU
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndxArray * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        // See if any of the above calls made errors
        checkError(*params.out, "TextField.cpp", __LINE__);

        // Request the font texture from the repository
        font.panelW = (uint32_t)(params.screenResX * params.charSize.w);
        font.panelH = (uint32_t)(params.screenResY * params.charSize.h);
        params.repo->request(params.fontName.c_str(), font, *params.out);

        // set these member variables for later use
        numIndicesToDraw = sizeIndxArray;
        numLines = (uint32_t)numCharsY;
        numCharsPerLine = (uint32_t)numCharsX;
        numCharsTotal = (uint32_t)(numCharsX * numCharsY);

        return true;
    }

    void TextField::setPosition(float x, float y) {
        position = {x, y};
    }

    void TextField::updateText(uint32_t start, const char* text) {
        std::string textStr(text);
        uint32_t end = (uint32_t)std::min((uint32_t)textStr.length(), numCharsTotal - start);
        std::vector<float> charOffsets(end * 8);
        for (uint32_t i = 0; i < end; ++i) {
            for (int j = 0; j < 8; j += 2) {
                charOffsets[i * 8 + j + 0] = (textStr[i] - font.firstChar + font.getOffsetToFirstChar() +
                                             (j == 2 || j == 6)) * font.getPanelAdvance();
                charOffsets[i * 8 + j + 1] = j / 4;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferSubData(GL_ARRAY_BUFFER, start * 8 * sizeof(float), charOffsets.size() * sizeof(float),
                        charOffsets.data());
    }

    void TextField::updateTextBack(uint32_t rStart, const char* text) {
        updateText(numCharsTotal - rStart, text);
    }

    void TextField::updateTextLine(uint32_t line, const char* text) {
        updateText(line * numCharsPerLine, text);
    }

    void TextField::updateTextLineBack(uint32_t rLine, const char* text) {
        updateTextBack((rLine + 1) * numCharsPerLine, text);
    }

    void TextField::draw() {
        glBindVertexArray(vaoLoc);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, font.texture);

        glUseProgram(shdrLoc);
        glUniform1i(txtrLoc_chars, 1);

        glUniform3fv(unifLoc_color, 1, &color[0]);
        glUniform2fv(unifLoc_postn, 1, &position[0]);
        glDrawElements(GL_TRIANGLES, (GLsizei)numIndicesToDraw, GL_UNSIGNED_INT, 0);
    }

    uint32_t TextField::getNumLines() {
        return numLines;
    }

    uint32_t TextField::getCharsPerLine() {
        return numCharsPerLine;
    }

}

