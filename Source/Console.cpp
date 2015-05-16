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
        lines.push_back(line);
    }
            
    std::cout << text;
    textHasChanged = true;
}

std::string Console::getLog(){
    std::string out;
    for(auto line:lines){
        out += line + '\n';
    }
    return out;
}

std::string Console::getLine(int line){
    if (lines.empty() || line < 0 || line > lines.size() - 1){
        return "";
    } else {
        return lines.at(line);
    }
}

std::string Console::getLast(){
    return getLine(lines.size() - 1);
}

void Console::setState(bool console, bool menu){
    consoleIsActive = console;
    menuIsActive = menu;
}

void Console::addToCommand(const char* text){
    pendingCommand += text;
    textHasChanged = true;
}

void Console::eraseOneCharFromCommand(){
    if (pendingCommand.empty()) return;
    pendingCommand.pop_back();
    textHasChanged = true;
}

void Console::clearCommand(){
    pendingCommand.clear();
    textHasChanged = true;
}

std::string Console::submitCommand(){
    if (pendingCommand.empty()) return "";
    lines.push_back((menuIsActive ? "MENU>: " : ">: ") + pendingCommand);
    std::cout << (menuIsActive ? "MENU>: " : ">: ") << pendingCommand << std::endl;
    std::string temp = pendingCommand;
    clearCommand();
    return temp;
}