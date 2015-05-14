/*******************************************************************************
 * File:   UserControl.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include "../Headers/UserControl.h"

using namespace DualityEngine;

//<editor-fold>

System_UserControl::System_UserControl(ComponentBank* bank, ControlDelegates* delegates)
                  : System(bank, "Control System", 1){
    this->delegates = delegates;
}

System_UserControl::~System_UserControl(){
    delegates = NULL;
}

bool System_UserControl::init(std::stringstream& output){
    requiredComponents.at(0) = CONTROL ;
    return true;
}


void System_UserControl::tick(){
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    
    //Handle events on queue
    while(SDL_PollEvent(&sdlEvent) != 0){
        //User requests quit
        if(sdlEvent.type == SDL_QUIT){
            delegates->quit();
            delegates->output("\nWindow was exited.\n\n");
        }
        else if(sdlEvent.type == SDL_KEYDOWN){
            if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                delegates->menu();
            }
            if(sdlEvent.key.keysym.sym == SDLK_BACKQUOTE){
                consoleIsActive = !consoleIsActive;
                consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
                delegates->clearCommand();
                delegates->output(consoleIsActive ? "console ON.\n" : "console OFF.\n");
            }
            if (consoleIsActive){
                if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE){
                    delegates->backspaceCommand();
                }
                else if(sdlEvent.key.keysym.sym == SDLK_RETURN){
                    std::string command = delegates->submitCommand();
                    parseCommand(command);
                }
                else if(sdlEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL){
                    SDL_SetClipboardText(delegates->getLastConsoleLine().c_str());
                }
                else if(sdlEvent.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL){
                    delegates->appendToCommand(SDL_GetClipboardText());                    
                }
            }
        }
        else if(sdlEvent.type == SDL_TEXTINPUT){
            if(!((keyStates[SDL_SCANCODE_C] || keyStates[SDL_SCANCODE_V])
                                            && SDL_GetModState() & KMOD_CTRL)){
                delegates->appendToCommand(sdlEvent.text.text);
            }
        }
    }
    
    if(!consoleIsActive){
        handleControlKeys(keyStates);
    }
}

void System_UserControl::parseCommand(std::string command){
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::vector<std::string> args;
    std::string temp;
    while (commandLine >> temp){
        args.push_back(temp);
        temp.clear();
    }
    if (args[0] == "exit" || args[0] == "quit"){
        delegates->quit();
    } else {
        std::string error = "Bad Command: " + args[0] + '\n';
        delegates->output(error.c_str());
    }
}

void System_UserControl::handleControlKeys(const Uint8* keyStates){
    
//    if(keyStates[SDL_SCANCODE_W]){
//        //do w thing.
//    }
}

//</editor-fold>