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
    log << text;
    
    std::string line;
    while (std::getline(log, line, '\n')){
        lines.push_back(line);
    }
            
    std::cout << text;
}

std::string Console::getLog(){
    std::string out;
    for(auto line:lines){
        out += line;
    }
    return out;
}

std::string Console::getLine(int line){
    return lines[line];
}