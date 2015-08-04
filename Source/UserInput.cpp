/*******************************************************************************
 * File:   UserInput.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include "../Headers/UserInput.h"
#include "ControlBase.h"

using namespace DualityEngine;


System_UserInput::System_UserInput(ComponentBank* bank)
                  : System(bank, "User Input System", 0){
    
}

System_UserInput::~System_UserInput(){
}

int filterFunction(SDL_Event* e){
    if (e != NULL){
        if (e->type == SDL_KEYDOWN || e->type == SDL_TEXTINPUT || e->type == SDL_QUIT){
            return 1;
        }
    }
    return 0;
}

bool System_UserInput::init(std::stringstream& output){
    
    // Initialize SDL overall.
    // This needs to be done in this thread instead of in the graphics thread to prevent
    // massive event handing slowdowns.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        output << "SDL did not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
        output << "SDL could not enable relative mouse mode: " << SDL_GetError() << std::endl;
        return false;
    }
    
    
//    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
//    if (SDL_EventState(SDL_MOUSEMOTION, SDL_QUERY)){
//        output << "Failed to set SDL mouse motion events to disabled state: " << SDL_GetError() << std::endl;
//        return false;
//    }
    return true;
}

void System_UserInput::tick(){
    
    // Get current keyboard state
    const Uint8* keyStates = SDL_GetKeyboardState(NULL);
    
    // Handle events on SDL event queue
    while(SDL_PollEvent(&sdlEvent) != 0){
        switch(sdlEvent.type){
            case(SDL_MOUSEMOTION):
                if (!consoleIsActive) {
                    applyMouseMotion(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
                }
                break;
            case(SDL_KEYDOWN):
                handleKeyDown(sdlEvent);
                break;
            case(SDL_TEXTINPUT):
                if (consoleIsActive){
                    if(!((keyStates[SDL_SCANCODE_C] || keyStates[SDL_SCANCODE_V])
                                                    && SDL_GetModState() & KMOD_CTRL)){
                        bank->dlgt->appendToCommand(sdlEvent.text.text);
                    }
                }
                break;
//            case(SDL_KEYUP):
//                break;
            case(SDL_QUIT):
                bank->dlgt->quit();
                bank->dlgt->output("\nFORCED EXIT\n\n");
                break;
            default:
//                bank->dlgt->outputStr(std::to_string(sdlEvent.type));
                break;
        }
    }

//    if(!consoleIsActive){
//        if (bank->activeControlID != DUA_NULL_ID){
//            checkMouseMotionManual();
//        }
//    }

    if(!consoleIsActive){
        if (bank->activeControlID != DUA_NULL_ID){
            handleControlKeys(keyStates);
        }
    }
}

void System_UserInput::handleMenuCommand(const std::string& command){
    
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

void System_UserInput::handleKeyDown(SDL_Event& event){
    if (consoleIsActive){
        switch(event.key.keysym.sym){
            case(SDLK_BACKQUOTE):
                backQuotePressed();
                break;
            case(SDLK_ESCAPE):
                escPressed();
                break;
            case(SDLK_BACKSPACE):
                bank->dlgt->backspaceCommand();
                break;
            case(SDLK_DELETE):
                bank->dlgt->deleteCommand();
                break;
            case(SDLK_RETURN):
            {
                std::string command = bank->dlgt->consoleEnter();
                if (MenuIsActive){
                    handleMenuCommand(command);
                } else {
                    bank->dlgt->submitScriptCommand(command);
                }
                break;
            }
            case(SDLK_UP):
                bank->dlgt->upOneCommand();
                break;
            case(SDLK_DOWN):
                bank->dlgt->downOneCommand();
                break;
            case(SDLK_LEFT):
                bank->dlgt->leftCursor();
                break;
            case(SDLK_RIGHT):
                bank->dlgt->rightCursor();
                break;
            case(SDLK_PAGEUP):
                bank->dlgt->logTraverse(1);
                break;
            case(SDLK_PAGEDOWN):
                bank->dlgt->logTraverse(-1);
                break;
            case(SDLK_c):
                if (SDL_GetModState() & KMOD_CTRL){
                    SDL_SetClipboardText(bank->dlgt->getCurrentLogLine().c_str());
                }
                break;
            case(SDLK_v):
                if (SDL_GetModState() & KMOD_CTRL){
                    bank->dlgt->appendToCommand(SDL_GetClipboardText());
                }
                break;
            default:
                break;
        }
    }else{
        switch(event.key.keysym.sym){
            case(SDLK_BACKQUOTE):
                backQuotePressed();
                break;
            case(SDLK_ESCAPE):
                escPressed();
                break;
        }
    }
}

void System_UserInput::handleControlKeys(const Uint8* keyStates){    
        
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
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHPOS, 1);
            }
            if (keyStates[SDL_SCANCODE_DOWN]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHNEG, 1);
            }
            if (keyStates[SDL_SCANCODE_LEFT]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWPOS, 1);
            }
            if (keyStates[SDL_SCANCODE_RIGHT]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWNEG, 1);
            }
            if (keyStates[SDL_SCANCODE_Q]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::ROLLPOS, 1);
            }
            if (keyStates[SDL_SCANCODE_E]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::ROLLNEG, 1);
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

void System_UserInput::checkMouseMotionManual(){    
    
    SDL_GetMouseState(&mouseX, &mouseY);
    bank->dlgt->setMousePos(0, 0);
    
    switch(bank->currentControlType){
        case ControlTypes::SPACE:
            if (mouseX > 0){
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWPOS, mouseX);
            } else {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWNEG, mouseX);
            }
            if (mouseY > 0){
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHPOS, mouseY);
            } else {
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHNEG, mouseY);
            }
            break;
        case ControlTypes::NONE:
            break;
        default:
            break;
    }
}

void System_UserInput::applyMouseMotion(int x, int y){
    switch(bank->currentControlType){
        case ControlTypes::SPACE:
            if (x >= 0){
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWNEG, x);
            } else {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWPOS, fabs(x));
            }
            if (y >= 0){
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHNEG, y);
            } else {
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHPOS, fabs(y));
            }
            break;
        case ControlTypes::NONE:
            break;
        default:
            break;
    }
}

void System_UserInput::backQuotePressed(){
    if (!MenuIsActive) {
        consoleIsActive = !consoleIsActive;
        consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
        bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
    }
}

void System_UserInput::escPressed(){
    MenuIsActive = !MenuIsActive;
    if (MenuIsActive) {
        if (!consoleIsActive) {
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