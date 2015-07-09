/*******************************************************************************
 * File:   Console.cpp
 * Author: Galen Cochrane *
 * Created on May 12, 2015, 9:15 PM
 * 
 * 
 ******************************************************************************/
#include "../Headers/Console.h"
using namespace DualityEngine;

Console::~Console(){
    //std::cout << getLog();
}

void Console::output(const char* text){
    std::stringstream temp(text);
    std::string line;
    while (std::getline(temp, line)){
        logLines.push_back(line);
    }
    bodyHasChangedVisually = true;
    
    #ifdef DUA_DEBUG_CONSOLE_TO_COUT
    std::cout << text;
    #endif
}

void Console::outputStr(const std::string& text){
    output(text.c_str());
}

std::string Console::getLog(){
    std::string out;
    for(auto line:logLines){
        out += line + '\n';
    }
    return out;
}

std::string Console::getLogLine(int line){
    if (logLines.empty() || line < 0 || line > logLines.size() - 1){
        return "";
    } else {
        return logLines.at(line);
    }
}

std::string Console::getLogLineFromBack(int lineFromBack){
    return getLogLine(logLines.size() - lineFromBack - 1);
}

std::string Console::getCurrentLogLine(){
    return getLogLineFromBack(logLineTraverser);
}

std::string Console::getPendingCommand(){
    return pendingCommand;
}

void Console::setState(bool console, bool menu){
    consoleIsActive = console;
    menuIsActive = menu;
    commHasChangedVisually = true;
    if (menu) logLineTraverser = 0;
}

void Console::addToCommand(const char* text){
    pendingCommand.insert(cursorPosition, text);
    cursorPosition += std::strlen(text);       
    if (submitLineActive == submitLinePending){
        submittedLines.at(submitLinePending) = pendingCommand;
    }
    commHasChangedVisually = true;
    //outputStr(std::to_string(cursorPosition));
    ///*DEBUG*/std::cout << pendingCommand << std::endl;
}

void Console::applyBackspace(){
    if (!pendingCommand.empty() && cursorPosition > 0){
        pendingCommand.erase(cursorPosition - 1, 1);
        cursorPosition--;
        commHasChangedVisually = true;
        ///*DEBUG*/std::cout <<  pendingCommand << std::endl;
    }
}

void Console::applyDelete(){
    if (!pendingCommand.empty() && cursorPosition < pendingCommand.length()){
        pendingCommand.erase(cursorPosition, 1);
        commHasChangedVisually = true;
        ///*DEBUG*/std::cout << pendingCommand << std::endl;
    }
}

void Console::clearCommand(){
    pendingCommand.clear();
    cursorPosition = 0;
    commHasChangedVisually = true;    
}

void Console::refreshPendingCommand(){
    pendingCommand = submittedLines.at(submitLineActive);
    cursorPosition = pendingCommand.size();
    commHasChangedVisually = true;
    ///*DEBUG*/std::cout << pendingCommand << std::endl;
}

void Console::upOneCommand(){
    if (submitLineActive > 0){
        submitLineActive--;
        refreshPendingCommand();        
    }
}

void Console::downOneCommand(){
    if (submitLineActive < submitLinePending){
        submitLineActive++;
        refreshPendingCommand();
    }
}

void Console::leftCursor(){
    if (cursorPosition > 0){
        cursorPosition--;
        commHasChangedVisually = true;
   }
}

void Console::rightCursor(){
    if (cursorPosition < pendingCommand.length()){
        cursorPosition++;
        commHasChangedVisually = true;
    }
}

void Console::traverseLog(int numLines){
    if (logLineTraverser + numLines < 0){
        logLineTraverser = 0;
    } else if (logLineTraverser + numLines > logLines.size() - 1){
        logLineTraverser = logLines.size() - 1;
    } else {
        logLineTraverser += numLines;
    }
    bodyHasChangedVisually = true;
}

std::string Console::submitCommand(){
    if (pendingCommand.empty()) return "";
    std::string temp = pendingCommand;
    clearCommand();
    logLineTraverser = 0;
    if ((submittedLines.size() <= 1) ? true : temp != submittedLines.at(submitLinePending - 1)){
        submittedLines.at(submitLinePending) = temp;
        submittedLines.push_back("");
        submitLinePending++;
    }
    submitLineActive = submitLinePending;
    outputStr((menuIsActive ? "MENU>: " : ">: ") + temp + "\n");
    
    return temp;
}