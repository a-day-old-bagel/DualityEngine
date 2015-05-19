/*******************************************************************************
 * File:   Console.cpp
 * Author: Galen Cochrane *
 * Created on May 12, 2015, 9:15 PM
 * 
 * 
 ******************************************************************************/
#include "../Headers/Console.h"
using namespace DualityEngine;

void Console::output(const char* text){
    std::stringstream temp(text);
    std::string line;
    while (std::getline(temp, line)){
        logLines.push_back(line);
    }
            
    std::cout << text;
    hasChangedVisually = true;
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

std::string Console::getLastLogLine(){
    return getLogLine(logLines.size() - 1);
}

void Console::setState(bool console, bool menu){
    consoleIsActive = console;
    menuIsActive = menu;
}

void Console::addToCommand(const char* text){
//    if (cursorPosition >= pendingCommand.length()){
//        pendingCommand += text;
//        if (submitLineActive == submitLinePending){
//            submittedLines.at(submitLinePending) = pendingCommand;
//        }
//        cursorPosition = pendingCommand.length();
//    } else {
        //int oldLength = pendingCommand.length();
        pendingCommand.insert(cursorPosition, text);
        //cursorPosition += pendingCommand.length() - oldLength;
        cursorPosition++;
        
        if (submitLineActive == submitLinePending){
            submittedLines.at(submitLinePending) = pendingCommand;
        }
//    }
    hasChangedVisually = true;
    /*DEBUG*/std::cout << pendingCommand << std::endl;
}

void Console::applyBackspace(){
    if (!pendingCommand.empty() && cursorPosition > 0){
        pendingCommand.erase(cursorPosition - 1, 1);
        cursorPosition--;
        hasChangedVisually = true;
        /*DEBUG*/std::cout <<  pendingCommand << std::endl;
    }
}

void Console::applyDelete(){
    if (!pendingCommand.empty() && cursorPosition < pendingCommand.length()){
        pendingCommand.erase(cursorPosition, 1);
        hasChangedVisually = true;
        /*DEBUG*/std::cout << pendingCommand << std::endl;
    }
}

void Console::clearCommand(){
    pendingCommand.clear();
    cursorPosition = 0;
    hasChangedVisually = true;    
}

void Console::refreshPendingCommand(){
    pendingCommand = submittedLines.at(submitLineActive);
    cursorPosition = pendingCommand.size();
    hasChangedVisually = true;
    /*DEBUG*/std::cout << pendingCommand << std::endl;
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
        hasChangedVisually = true;
   }
}

void Console::rightCursor(){
    if (cursorPosition < pendingCommand.length()){
        cursorPosition++;
        hasChangedVisually = true;
    }
}

std::string Console::submitCommand(){
    if (pendingCommand.empty()) return "";
    std::string temp = pendingCommand;
    pendingCommand.clear();
    cursorPosition = 0;
    if ((submittedLines.size() <= 1) ? true : temp != submittedLines.at(submitLinePending - 1)){
        submittedLines.at(submitLinePending) = temp;
        submittedLines.push_back("");
        submitLinePending++;
    }
    submitLineActive = submitLinePending;
    outputStr((menuIsActive ? "MENU>: " : ">: ") + temp + "\n");
    
    return temp;
}