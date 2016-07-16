//
// Created by volundr on 7/15/16.
//

#ifndef DUALITYENGINE_CONSOLEDRAWER_H
#define DUALITYENGINE_CONSOLEDRAWER_H

#include "Console.h"
#include "TextField.h"

namespace DualityEngine {
    class ConsoleDrawer {
        Console* console;
        TextField text;
        uint32_t numLinesInCommand = 0;
    public:
        bool init(TextFieldParams& textParams, Console* console);
        void setPosition(float x, float y);
        void updateText();
        void draw();
    };
}

#endif //DUALITYENGINE_CONSOLEDRAWER_H
