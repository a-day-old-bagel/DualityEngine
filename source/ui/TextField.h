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

        struct {
            unsigned float x;
            unsigned float y;
        } spacing;  // portion of char size
        struct {
            unsigned float w;
            unsigned float h;
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

        float getAdvanceX();
        float getAdvanceY();
    };

    class TextField {
        FontDescriptor font;
    public:
        bool init(TextFieldParams& params);
    };
}

#endif //DUALITYENGINE_TEXTLINE_H
