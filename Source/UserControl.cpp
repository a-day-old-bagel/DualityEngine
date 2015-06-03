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
                  : System(bank, "Control System", 0){
    this->dlgt = delegates;
}

System_UserControl::~System_UserControl(){
    dlgt = NULL;
}

bool System_UserControl::init(std::stringstream& output){
    return true;
}


void System_UserControl::tick(){
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    
    //Handle events on queue
    while(SDL_PollEvent(&sdlEvent) != 0){
        //User requests quit
        if(sdlEvent.type == SDL_QUIT){
            dlgt->quit();
            dlgt->output("\nFORCED EXIT\n\n");
        }
        else if(sdlEvent.type == SDL_KEYDOWN){
            if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                MenuIsActive = !MenuIsActive;
                if (MenuIsActive){
                    if (!consoleIsActive){
                        consoleIsActive = true;
                        SDL_StartTextInput();
                    }
                    dlgt->clearCommand();
                    dlgt->output(menuText.c_str());
                    dlgt->pause();
                } else {
                    consoleIsActive = false;
                    SDL_StopTextInput();
                    dlgt->resume();
                }
                dlgt->setConsoleState(consoleIsActive, MenuIsActive);
            }
            if(sdlEvent.key.keysym.sym == SDLK_BACKQUOTE){
                if (!MenuIsActive){
                    consoleIsActive = !consoleIsActive;
                    consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
                    dlgt->setConsoleState(consoleIsActive, MenuIsActive);
                }
            }
            if (consoleIsActive){
                if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE){
                    dlgt->backspaceCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_DELETE){
                    dlgt->deleteCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_RETURN){
                    std::string command = dlgt->consoleEnter();
                    if (MenuIsActive){
                        handleMenuCommand(command);
                    } else {
                        dlgt->submitScriptCommand(command);
                    }
                } else if(sdlEvent.key.keysym.sym == SDLK_UP){
                    dlgt->upOneCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_DOWN){
                    dlgt->downOneCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_LEFT){
                    dlgt->leftCursor();
                } else if(sdlEvent.key.keysym.sym == SDLK_RIGHT){
                    dlgt->rightCursor();
                } else if(sdlEvent.key.keysym.sym == SDLK_PAGEUP){
                    dlgt->logTraverse(1);
                } else if(sdlEvent.key.keysym.sym == SDLK_PAGEDOWN){
                    dlgt->logTraverse(-1);
                } else if(sdlEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL){
                    SDL_SetClipboardText(dlgt->getLogFromBack(0).c_str());
                } else if(sdlEvent.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL){
                    dlgt->appendToCommand(SDL_GetClipboardText());                    
                }
            }
        }
        else if(sdlEvent.type == SDL_TEXTINPUT){
            if(!((keyStates[SDL_SCANCODE_C] || keyStates[SDL_SCANCODE_V])
                                            && SDL_GetModState() & KMOD_CTRL)){
                dlgt->appendToCommand(sdlEvent.text.text);
            }
        }
    }
    
    if(!consoleIsActive){
        handleControlKeys(keyStates);
    }
}

void System_UserControl::handleMenuCommand(const std::string& command){
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::vector<std::string> args;
    std::string temp;
    while (commandLine >> temp){
        args.push_back(temp);
        temp.clear();
    }
    const int numArgs = args.size();
    if (numArgs < 1) return;
    
    if (args[0] == "exit"){
        dlgt->quit();
    } else if (args[0] == "new"){
        dlgt->output("new game command not yet implemented\n");
    } else if (args[0] == "load"){
        dlgt->output("load game command not yet implemented\n");
    } else if (args[0] == "save"){
        dlgt->output("save game command not yet implemented\n");
    } else if (args[0] == "help"){
        dlgt->output("You're in the menu. Type one of the following: 'new', 'load', 'save', or 'exit'.\nPress ESC to exit the menu.\n");
    } else {
        dlgt->outputStr("Not a menu option: " + args[0]);
    }
}

void System_UserControl::handleControlKeys(const Uint8* keyStates){
    
//    if(keyStates[SDL_SCANCODE_W]){
//        //do w thing.
//    }
}

//</editor-fold>