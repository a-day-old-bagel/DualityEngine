//
// Created by volundr on 7/11/16.
//

#include "TextField.h"

namespace DualityEngine {

    float TextFieldParams::getAdvanceX() {
        if (initType != FIELD_SIZE_CHAR_SIZE) {
            return std::numeric_limits<float>::quiet_NaN();
        }
        return charSize.w + charSize.w * spacing.x;
    }

    float TextFieldParams::getAdvanceY() {
        if (initType != FIELD_SIZE_CHAR_SIZE) {
            return std::numeric_limits<float>::quiet_NaN();
        }
        return charSize.h + charSize.h * spacing.y;
    }


    bool TextField::init(TextFieldParams& params) {

        float bodyW, bodyH;
        float marginW, marginH;
        uint32_t numCharsX, numCharsY;

        switch(params.initType) {
            case TextFieldParams::FIELD_SIZE_CHAR_SIZE:
                numCharsX = (uint32_t)(params.fieldSize.w / params.getAdvanceX());
                numCharsY = (uint32_t)(params.fieldSize.h / params.getAdvanceY());
                bodyW = params.charSize.w + (params.getAdvanceX() * (numCharsX - 1));
                bodyH = params.charSize.h + (params.getAdvanceY() * (numCharsY - 1));
                marginW = 0.5f * (params.fieldSize.w - bodyW);
                marginH = 0.5f * (params.fieldSize.h - bodyH);
                break;
            case TextFieldParams::CHAR_NUM_CHAR_SIZE:
                numCharsX = params.numChars.x;
                numCharsY = params.numChars.y;
                break;
            case TextFieldParams::INVALID:
            default:
                *params.out << "<!>    TextFieldParam initType invalid!\n";
                return false;
        }

        font.panelW = (uint32_t)(params.screenResX * params.charSize.w);
        font.panelH = (uint32_t)(params.screenResY * params.charSize.h);
        params.repo->request(params.fontName.c_str(), font, *params.out);



        return true;
    }

}

