/*******************************************************************************
 * File:   UserControl.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include "../Headers/UserControl.h"
#include "ControlBase.h"

using namespace DualityEngine;


System_UserControl::System_UserControl(ComponentBank* bank)
                  : System(bank, "Control System", 0){
    
}

System_UserControl::~System_UserControl(){
}

bool System_UserControl::init(std::stringstream& output){
//    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
//        output << "SDL could not enable relative mouse mode: " << SDL_GetError() << std::endl;
//        return false;
//    }
    return true;
}

void System_UserControl::tick(){
    
    // Get current keyboard state
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    
    // Handle events on SDL event queue
    while(SDL_PollEvent(&sdlEvent) != 0){
//        
//        if(sdlEvent.type == SDL_MOUSEMOTION){
//            if(!consoleIsActive){
//                if (bank->activeControlID != DUA_NULL_ID){
//                    recordMouseMotion(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
//                }
//            }
//        }else if(sdlEvent.type == SDL_KEYDOWN){
        if(sdlEvent.type == SDL_KEYDOWN){
            if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                MenuIsActive = !MenuIsActive;
                if (MenuIsActive){
                    if (!consoleIsActive){
                        consoleIsActive = true;
                        SDL_StartTextInput();
                    }
                    bank->dlgt->clearCommand();
                    bank->dlgt->output(menuText.c_str());
                    bank->dlgt->pause();
                } else {
                    consoleIsActive = false;
                    SDL_StopTextInput();
                    bank->dlgt->resume();
                }
                bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
            }else if(sdlEvent.key.keysym.sym == SDLK_BACKQUOTE){
                if (!MenuIsActive){
                    consoleIsActive = !consoleIsActive;
                    consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
                    bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
                }
            }else if (consoleIsActive){
                if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE){
                    bank->dlgt->backspaceCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_DELETE){
                    bank->dlgt->deleteCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_RETURN){
                    std::string command = bank->dlgt->consoleEnter();
                    if (MenuIsActive){
                        handleMenuCommand(command);
                    } else {
                        bank->dlgt->submitScriptCommand(command);
                    }
                } else if(sdlEvent.key.keysym.sym == SDLK_UP){
                    bank->dlgt->upOneCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_DOWN){
                    bank->dlgt->downOneCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_LEFT){
                    bank->dlgt->leftCursor();
                } else if(sdlEvent.key.keysym.sym == SDLK_RIGHT){
                    bank->dlgt->rightCursor();
                } else if(sdlEvent.key.keysym.sym == SDLK_PAGEUP){
                    bank->dlgt->logTraverse(1);
                } else if(sdlEvent.key.keysym.sym == SDLK_PAGEDOWN){
                    bank->dlgt->logTraverse(-1);
                } else if(sdlEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL){
                    SDL_SetClipboardText(bank->dlgt->getCurrentLogLine().c_str());
                } else if(sdlEvent.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL){
                    bank->dlgt->appendToCommand(SDL_GetClipboardText());                    
                }
            }else{
                handleKeyDown(sdlEvent);
            }
        }else if(sdlEvent.type == SDL_TEXTINPUT){
            if (consoleIsActive){
                if(!((keyStates[SDL_SCANCODE_C] || keyStates[SDL_SCANCODE_V])
                                                && SDL_GetModState() & KMOD_CTRL)){
                    bank->dlgt->appendToCommand(sdlEvent.text.text);
                }
            }
        }else if(sdlEvent.type == SDL_QUIT){
            bank->dlgt->quit();
            bank->dlgt->output("\nFORCED EXIT\n\n");
        }
    }

    if(!consoleIsActive){
        if (bank->activeControlID != DUA_NULL_ID){
            handleMouseMotion();
        }
    }

    if(!consoleIsActive){
        if (bank->activeControlID != DUA_NULL_ID){
            handleControlKeys(keyStates);
        }
    }
}

void System_UserControl::handleMenuCommand(const std::string& command){
    
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::string arg0;
    commandLine >> arg0;
    
    if (arg0 == "exit" || arg0 == "new" || arg0 == "save" || arg0 == "load"){
        bank->dlgt->submitScriptCommand(command);
    } else if (arg0 == "help"){        
        bank->dlgt->submitScriptCommand(command);
    } else {
        bank->dlgt->outputStr("Not a menu option: " + arg0 + ". Only commands 'new', 'load', 'save', 'exit', and 'help' may be accessed from the menu. Press ESC to leave the menu. ~ key accesses console during play.");
    }
}

void System_UserControl::handleKeyDown(SDL_Event& event){
    
}

void System_UserControl::handleControlKeys(const Uint8* keyStates){    
        
    switch(bank->currentControlType){
        case ControlTypes::SPACE:
            if (keyStates[SDL_SCANCODE_W]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::FORWARD, 1);
            }
            if (keyStates[SDL_SCANCODE_S]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::BACKWARD, 1);
            }
            if (keyStates[SDL_SCANCODE_A]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::LEFT, 1);
            }
            if (keyStates[SDL_SCANCODE_D]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::RIGHT, 1);
            }
            if (keyStates[SDL_SCANCODE_LSHIFT]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::UP, 1);
            }
            if (keyStates[SDL_SCANCODE_LCTRL]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::DOWN, 1);
            }
            if (keyStates[SDL_SCANCODE_UP]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCH, 1);
            }
            if (keyStates[SDL_SCANCODE_DOWN]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCH, -1);
            }
            if (keyStates[SDL_SCANCODE_LEFT]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAW, 1);
            }
            if (keyStates[SDL_SCANCODE_RIGHT]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAW, -1);
            }
            if (keyStates[SDL_SCANCODE_Q]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::ROLL, 1);
            }
            if (keyStates[SDL_SCANCODE_E]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::ROLL, -1);
            }
            if (keyStates[SDL_SCANCODE_SPACE]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::LINBRAKE, 1);
            }
            if (keyStates[SDL_SCANCODE_LALT]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::ANGBRAKE, 1);
            }
            break;
        case ControlTypes::NONE:
            break;
        default:
            break;
    }       
}

void System_UserControl::handleMouseMotion(){    
    
    SDL_GetMouseState(&mouseX, &mouseY);
    bank->dlgt->setMousePos(0, 0);
    
    switch(bank->currentControlType){
        case ControlTypes::SPACE:
            bank->pSpaceControlCurrent->applyInput(ControlSS::PITCH, mouseY);
            bank->pSpaceControlCurrent->applyInput(ControlSS::YAW, mouseX);
//            mouseX = 0;
//            mouseY = 0;
            break;
        case ControlTypes::NONE:
            break;
        default:
            break;
    }
}