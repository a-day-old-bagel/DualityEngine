//
// Created by volundr on 7/15/16.
//

#include "ConsoleDrawer.h"

namespace DualityEngine {

    bool ConsoleDrawer::init(TextFieldParams& textParams, Console* console) {
        this->console = console;
        bool result = text.init(textParams);
        console->wrapCharsPerLine(text.getCharsPerLine());
        return result;
    }

    void ConsoleDrawer::setPosition(float x, float y) {
        text.setPosition(x, y);
    }

    void ConsoleDrawer::updateText() {
        if (console->commHasChangedVisually) {
            console->commHasChangedVisually = false;
            std::string currComm = console->getPendingCommand();
            currComm.insert(0, console->menuIsActive ? Settings::Console::menuPrompt: Settings::Console::consolePrompt);
            std::vector<std::string> commLines;
            uint32_t charsPerLine = text.getCharsPerLine();
            for (uint32_t i = 0; i < currComm.length() / charsPerLine + 1; ++i) {
                commLines.push_back(currComm.substr((size_t) (charsPerLine * i), (size_t) charsPerLine));
            }
            commLines.back().append(charsPerLine - commLines.back().length(), ' ');
            numLinesInCommand = (uint32_t)commLines.size();
            std::stringstream combinedLines;
            for (uint32_t i = 0; i < commLines.size(); ++i) {
                combinedLines << commLines[i];
            }
            text.updateTextLineBack(numLinesInCommand - 1, combinedLines.str().c_str());
            console->bodyHasChangedVisually = true;
        }
        if (console->bodyHasChangedVisually) {
            console->bodyHasChangedVisually = false;
            uint32_t linesForBody = text.getNumLines() - numLinesInCommand;
            std::vector<std::string> bodyLines;
            for (int64_t i = linesForBody - 1; i >= 0; --i) {
                bodyLines.push_back(console->getLogLineFromBack((uint32_t)i + console->logLineTraverser));
                bodyLines.back().append(text.getCharsPerLine() - bodyLines.back().length(), ' ');
            }
            std::stringstream combinedLines;
            for (uint32_t i = 0; i < bodyLines.size(); ++i) {
                combinedLines << bodyLines[i];
            }
            text.updateText(0, combinedLines.str().c_str());
        }
    }

    void ConsoleDrawer::draw() {
        updateText();
        text.draw();
    }

}

