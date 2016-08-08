/*******************************************************************************
 * File:   Console.cpp
 * Author: Galen Cochrane *
 * Created on May 12, 2015, 9:15 PM
 * 
 * 
 ******************************************************************************/
#include "Console.h"
#include <thread>
using namespace DualityEngine;

Console::Console() {
    submittedLines = {""};
    pendingCommand = "";
    submitLinePending = 0;
    submitLineActive = 0;
    numCharsPerLine = 20;
    cursorPosition = 0;
    logLineTraverser = 0;
    consoleIsActive = false;
    menuIsActive = false;
	consoleIsFresh = true;
    bodyHasChangedVisually = true;
    commHasChangedVisually = true;
    wrapReady = false;
}
Console::~Console() {

}

void Console::output(const char* text) {
#ifdef DUA_DEBUG_CONSOLE_TO_COUT
    std::cout << text;
#endif
    if (!wrapReady) {
        preWrapReadyBuffer << text;
        return;
    }
    std::stringstream temp(text);
    std::string line;
    while (std::getline(temp, line)) {
        if ((int)line.length() > numCharsPerLine) {
            for (int i = 0; i < (int)line.length() / numCharsPerLine + 1; ++i) {
                std::string wrappedLine = line.substr((size_t)(numCharsPerLine * i), (size_t)numCharsPerLine);
                logLines.push_back(wrappedLine);
            }
        } else {
            logLines.push_back(line);
        }
    }
    bodyHasChangedVisually = true;
}

void Console::outputStr(const std::string& text) {
    output(text.c_str());
}

std::string Console::getLog() {
    std::stringstream out;
    out << preWrapReadyBuffer.str();
    for(auto line : logLines){
        out << line << std::endl;
    }
    return out.str();
}

std::string Console::getLogLine(int line) {
    if (logLines.empty() || line < 0 || line > (int)logLines.size() - 1){
        return "";
    } else {
        return logLines.at(line);
    }
}

std::string Console::getLogLineFromBack(int lineFromBack) {
    return getLogLine(logLines.size() - lineFromBack - 1);
}

std::string Console::getCurrentLogLine() {
    return getLogLineFromBack(logLineTraverser);
}

std::string Console::getPendingCommand() {
    return pendingCommand;
}

void Console::setState(bool console, bool menu) {
    consoleIsActive = console;
    menuIsActive = menu;
    commHasChangedVisually = true;
    if (menu) logLineTraverser = 0;
	if (!console) {
		consoleIsFresh = true;
	}
}

void Console::addToCommand(const char* text) {
	if (!(consoleIsFresh && text[0] == '`' ) ) {
		pendingCommand.insert(cursorPosition, text);
		cursorPosition += std::strlen(text);
		if (submitLineActive == submitLinePending) {
			submittedLines.at(submitLinePending) = pendingCommand;
		}
		commHasChangedVisually = true;
	}
	consoleIsFresh = false;
}

void Console::applyBackspace() {
    if (!pendingCommand.empty() && cursorPosition > 0){
        pendingCommand.erase(cursorPosition - 1, 1);
        cursorPosition--;
        commHasChangedVisually = true;
    }
}

void Console::applyDelete() {
    if (!pendingCommand.empty() && cursorPosition < (int)pendingCommand.length()){
        pendingCommand.erase(cursorPosition, 1);
        commHasChangedVisually = true;
    }
}

void Console::clearCommand() {
    pendingCommand.clear();
    cursorPosition = 0;
    commHasChangedVisually = true;    
}

void Console::refreshPendingCommand() {
    pendingCommand = submittedLines.at(submitLineActive);
    cursorPosition = pendingCommand.size();
    commHasChangedVisually = true;
}

void Console::upOneCommand() {
    if (submitLineActive > 0) {
        submitLineActive--;
        refreshPendingCommand();        
    }
}

void Console::downOneCommand() {
    if (submitLineActive < submitLinePending) {
        submitLineActive++;
        refreshPendingCommand();
    }
}

void Console::leftCursor() {
    if (cursorPosition > 0) {
        cursorPosition--;
        commHasChangedVisually = true;
    }
}

void Console::rightCursor() {
    if (cursorPosition < (int)pendingCommand.length()) {
        cursorPosition++;
        commHasChangedVisually = true;
    }
}

void Console::traverseLog(int numLines) {
    if (logLineTraverser + numLines < 0) {
        logLineTraverser = 0;
    } else if (logLineTraverser + numLines > (int)logLines.size() - 1) {
        logLineTraverser = logLines.size() - 1;
    } else {
        logLineTraverser += numLines;
    }
    bodyHasChangedVisually = true;
}

std::string Console::submitCommand() {
    if (pendingCommand.empty()) return "";
    std::string temp = pendingCommand;
    clearCommand();
    logLineTraverser = 0;
    if ((submittedLines.size() <= 1) ? true : temp != submittedLines.at(submitLinePending - 1)) {
        submittedLines.at(submitLinePending) = temp;
        submittedLines.push_back("");
        submitLinePending++;
    }
    submitLineActive = submitLinePending;
    outputStr((menuIsActive ? Settings::Console::menuPrompt : Settings::Console::consolePrompt) + temp + "\n");
    
    return temp;
}

void Console::wrapCharsPerLine(int charsPerLine) {
    numCharsPerLine = charsPerLine;
    wrapReady = true;
    outputStr(preWrapReadyBuffer.str());
    preWrapReadyBuffer.str("");
    preWrapReadyBuffer.clear();
}





