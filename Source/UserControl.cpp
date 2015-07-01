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

System_UserControl::System_UserControl(ComponentBank* bank)//, ControlDelegates* delegates)
                  : System(bank, "Control System", 0){
//    this->dlgt = delegates;
}

System_UserControl::~System_UserControl(){
//    dlgt = NULL;
    delete pDummyControl;
    delete pDummyPosition;
    delete pDummyOrientation;
    pDummyControl = NULL;
    pDummyPosition = NULL;
    pDummyOrientation = NULL;
}

bool System_UserControl::init(std::stringstream& output){
    pDummyControl = new Control();
    pDummyPosition = new Position(0,0,0);
    pDummyOrientation = new Orientation(0,0,0);
    return true;
}

void System_UserControl::clean(){
    System::clean();
    DUA_id localActiveControl = DUA_NULL_ID;
    pControlCurrent = pDummyControl;
    pPositionCurrent = pDummyPosition;
    pOrientationCurrent = pDummyOrientation;
}

void System_UserControl::tick(){
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    
    //Handle events on queue
    while(SDL_PollEvent(&sdlEvent) != 0){
        //User requests quit
        if(sdlEvent.type == SDL_QUIT){
            bank->dlgt->quit();
            bank->dlgt->output("\nFORCED EXIT\n\n");
        }
        else if(sdlEvent.type == SDL_KEYDOWN){
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
            }
            if(sdlEvent.key.keysym.sym == SDLK_BACKQUOTE){
                if (!MenuIsActive){
                    consoleIsActive = !consoleIsActive;
                    consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
                    bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
                }
            }
            if (consoleIsActive){
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
            }
        }
        else if(sdlEvent.type == SDL_TEXTINPUT){
            if (consoleIsActive){
                if(!((keyStates[SDL_SCANCODE_C] || keyStates[SDL_SCANCODE_V])
                                                && SDL_GetModState() & KMOD_CTRL)){
                    bank->dlgt->appendToCommand(sdlEvent.text.text);
                }
            }
        }
    }
    
    checkActiveControl();
    if(!consoleIsActive){
        handleControlKeys(keyStates);
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
        bank->dlgt->outputStr("Not a menu option: " + arg0 + ". Only the commands 'new', 'load', 'save', and 'exit' may be accessed from the menu. Press ESC to exit the menu, or type \"help\".");
    }
}

void System_UserControl::checkActiveControl(){
    if (bank->activeControlID != localActiveControl) {
        localActiveControl = bank->activeControlID;
        if (localActiveControl != DUA_NULL_ID) {
            pPositionCurrent = bank->getPositionPtr(localActiveControl);
            pControlCurrent = bank->getControlPtr(localActiveControl);      // CHANGE BANK::SWITCHTOCONTROL ONCE THINGS ARE BETTER
            pOrientationCurrent = bank->getOrientationPtr(localActiveControl);
        } else {
            pPositionCurrent = pDummyPosition;
            pControlCurrent = pDummyControl;
            pOrientationCurrent = pDummyOrientation;
        }
    }
}

void System_UserControl::handleControlKeys(const Uint8* keyStates){
    
    if (localActiveControl != DUA_NULL_ID){
        if(keyStates[SDL_SCANCODE_W]){
                pPositionCurrent->translate(DUA_vec3(pControlCurrent->forward) * 0.008f);
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_S]){
                pPositionCurrent->translate(DUA_vec3(pControlCurrent->forward) * -0.008f);
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_A]){
                pPositionCurrent->translate(DUA_vec3(pControlCurrent->right) * -0.008f);
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_D]){
                pPositionCurrent->translate(DUA_vec3(pControlCurrent->right) * 0.008f);
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_LSHIFT]){
                pPositionCurrent->translate(DUA_vec3(pControlCurrent->up) * 0.008f);
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_LCTRL]){
                pPositionCurrent->translate(DUA_vec3(pControlCurrent->up) * -0.008f);
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_UP]){
                pOrientationCurrent->rotate(DUA_vec3(0.005, 0, 0));
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_DOWN]){
                pOrientationCurrent->rotate(DUA_vec3(-0.005, 0, 0));
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_LEFT]){
                pOrientationCurrent->rotate(DUA_vec3(0, 0.005, 0));
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
        if(keyStates[SDL_SCANCODE_RIGHT]){
                pOrientationCurrent->rotate(DUA_vec3(0, -0.005, 0));
                bank->stateOn(localActiveControl, RECALCVIEWMAT);
        }
    }
}

//</editor-fold>