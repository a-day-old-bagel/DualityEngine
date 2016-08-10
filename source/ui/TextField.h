//
// Created by volundr on 7/11/16.
//

#ifndef DUALITYENGINE_TEXTLINE_H
#define DUALITYENGINE_TEXTLINE_H

//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);

#include "FontRepo.h"

namespace DualityEngine {

    struct TextFieldParams {
        FontRepo* repo;
        std::stringstream* out;
        std::string fontName;
        uint32_t screenResX;
        uint32_t screenResY;
        bool sdf = false;

        struct {
            float x;
            float y;
        } spacing;  // portion of char size
        struct {
            float w;
            float h;
        } charSize; // portion of screen size

        enum {
            INVALID,
            FIELD_SIZE_CHAR_SIZE,
            CHAR_NUM_CHAR_SIZE,
        } initType = INVALID;
        union {
            struct {
                float w;
                float h;
            } fieldSize;
            struct {
                uint32_t x;
                uint32_t y;
            } numChars;
        };

        float getAdvX();
        float getAdvY();
    };

    class TextField {
        FontDescriptor font;
        GLuint buffers[3];
        GLuint vaoLoc;
        GLuint shdrLoc;
        GLint txtrLoc_chars;
        GLint attrLoc_verts;
        GLint attrLoc_uvCoo;
        GLint unifLoc_postn;
        GLint unifLoc_color;
        uint64_t numIndicesToDraw;
        uint32_t numLines;
        uint32_t numCharsPerLine;
        uint32_t numCharsTotal;
        glm::vec2 position = {0.f, 0.f};
        glm::vec3 color = {1.f, 1.f, 1.f};
    public:
        bool init(TextFieldParams& params);
        void setPosition(float x, float y);
        void updateText(uint32_t start, const char* text);
        void updateTextBack(uint32_t rStart, const char* text);
        void updateTextLine(uint32_t line, const char* text);
        void updateTextLineBack(uint32_t rLine, const char* text);
        void draw();

        uint32_t getNumLines();
        uint32_t getCharsPerLine();
        FontDescriptor getFont();
    };
}

#endif //DUALITYENGINE_TEXTLINE_H
