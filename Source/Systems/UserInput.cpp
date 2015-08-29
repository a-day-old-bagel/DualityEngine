/*******************************************************************************
 * File:   UserInput.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include "UserInput.h"

#include "ControlTypes.h"

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
    // massive event handing slowdowns. Will see if works in Windows.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        output << "<!>    SDL did not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
#ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
    if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
        output << "<!>    SDL could not enable relative mouse mode: " << SDL_GetError() << std::endl;
        return false;
    }
#endif
    
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
                    handleMouseMotion(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
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
                bank->dlgt->output("\n<!>    FORCED EXIT\n\n");
                break;
            default:
//                bank->dlgt->outputStr(std::to_string(sdlEvent.type));
                break;
        }
    }

    if(!consoleIsActive){
        if (bank->activeControlID != DUA_NULL_ID){
            handleControlKeys(keyStates);
        }
    }
}

void System_UserInput::parseMenuCommand(const std::string& command){
    
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::string arg0;
    commandLine >> arg0;
    
    if (arg0 == "exit" || arg0 == "new" || arg0 == "save" || arg0 == "load"){
        bank->dlgt->submitScriptCommand(command);
    } else if (arg0 == "help"){        
        bank->dlgt->submitScriptCommand(command);
    } else {
        bank->dlgt->outputStr("<!>    Not a menu option: " + arg0 + ". Only commands 'new', 'load', 'save', 'exit', and 'help' may be accessed from the menu. Press ESC to leave the menu. ~ key accesses console during play.");
    }
}

void System_UserInput::handleKeyDown(SDL_Event& event){
    if (consoleIsActive){
        switch(event.key.keysym.sym){
            case(SDLK_BACKQUOTE):
                keyPressed_backQuote();
                return;
            case(SDLK_ESCAPE):
                keyPressed_esc();
                return;
            case(SDLK_BACKSPACE):
                bank->dlgt->backspaceCommand();
                return;
            case(SDLK_DELETE):
                bank->dlgt->deleteCommand();
                return;
            case(SDLK_RETURN):
            {
                std::string command = bank->dlgt->consoleEnter();
                if (MenuIsActive){
                    parseMenuCommand(command);
                } else {
                    bank->dlgt->submitScriptCommand(command);
                }
                return;
            }
            case(SDLK_UP):
                bank->dlgt->upOneCommand();
                return;
            case(SDLK_DOWN):
                bank->dlgt->downOneCommand();
                return;
            case(SDLK_LEFT):
                bank->dlgt->leftCursor();
                return;
            case(SDLK_RIGHT):
                bank->dlgt->rightCursor();
                return;
            case(SDLK_PAGEUP):
                bank->dlgt->logTraverse(1);
                return;
            case(SDLK_PAGEDOWN):
                bank->dlgt->logTraverse(-1);
                return;
            case(SDLK_c):
                if (SDL_GetModState() & KMOD_CTRL){
                    SDL_SetClipboardText(bank->dlgt->getCurrentLogLine().c_str());
                }
                return;
            case(SDLK_v):
                if (SDL_GetModState() & KMOD_CTRL){
                    bank->dlgt->appendToCommand(SDL_GetClipboardText());
                }
                return;
            default:
                return;
        }
    }else{
        switch(bank->currentControlType){
            case ControlTypes::SPACE:
                switch(event.key.keysym.sym){
                    case(SDLK_l):
                        bank->pSpaceControlCurrent->autoBrakeLinear = !bank->pSpaceControlCurrent->autoBrakeLinear;
                        return;
                    case(SDLK_k):
                        bank->pSpaceControlCurrent->autoBrakeAngular = !bank->pSpaceControlCurrent->autoBrakeAngular;
                        return;
                }
                break;
            case ControlTypes::NONE:
                break;
            default:
                break;
        }
        switch(event.key.keysym.sym){            
            case(SDLK_BACKQUOTE):
                keyPressed_backQuote();
                return;
            case(SDLK_ESCAPE):
                keyPressed_esc();
                return;
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
            if (keyStates[SDL_SCANCODE_C]) {
                bank->pSpaceControlCurrent->applyInput(ControlSS::ANGBRAKE, 1);
            }
            break;
        case ControlTypes::NONE:
            break;
        default:
            break;
    }       
}

void System_UserInput::handleMouseMotion(int x, int y){
    switch(bank->currentControlType){
        case ControlTypes::SPACE:
            if (x >= 0){
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWNEG, x * 0.06f);
            } else {
                bank->pSpaceControlCurrent->applyInput(ControlSS::YAWPOS, fabs(x) * 0.06f);
            }
            if (y >= 0){
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHNEG, y * 0.06f);
            } else {
                bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHPOS, fabs(y) * 0.06f);
            }
            break;
        case ControlTypes::NONE:
            break;
        default:
            break;
    }
}

void System_UserInput::keyPressed_backQuote(){
    if (!MenuIsActive) {
        consoleIsActive = !consoleIsActive;
        consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
#ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
        SDL_SetRelativeMouseMode(consoleIsActive ? SDL_FALSE : SDL_TRUE);
#endif
        bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
    }
}

void System_UserInput::keyPressed_esc(){
    MenuIsActive = !MenuIsActive;
    if (MenuIsActive) {
        if (!consoleIsActive) {
            consoleIsActive = true;
            SDL_StartTextInput();
#ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
            SDL_SetRelativeMouseMode(SDL_FALSE);
#endif
        }
        bank->dlgt->clearCommand();
        bank->dlgt->output(menuText.c_str());
        bank->dlgt->pause();
    } else {
        consoleIsActive = false;
        SDL_StopTextInput();
#ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
        SDL_SetRelativeMouseMode(SDL_TRUE);
#endif
        bank->dlgt->resume();
    }
    bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
}