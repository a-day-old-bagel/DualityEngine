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
    this->dlgt = delegates;
}

System_UserControl::~System_UserControl(){
    dlgt = NULL;
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
            dlgt->quit();
            dlgt->output("\nWindow was exited.\n\n");
        }
        else if(sdlEvent.type == SDL_KEYDOWN){
            if(sdlEvent.key.keysym.sym == SDLK_ESCAPE){
                //delegates->menu();
                MenuIsActive = !MenuIsActive;
                if (MenuIsActive){
                    if (!consoleIsActive){
                        consoleIsActive = true;
                        SDL_StartTextInput();
                    }
                    dlgt->clearCommand();
                    dlgt->output(menuText.c_str());
                    //presentTextMenu();
                } else {
                    consoleIsActive = false;
                    SDL_StopTextInput();
                }
                dlgt->setConsoleState(consoleIsActive, MenuIsActive);
            }
            if(sdlEvent.key.keysym.sym == SDLK_BACKQUOTE){
                if (!MenuIsActive){
                    consoleIsActive = !consoleIsActive;
                    consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
                    dlgt->clearCommand();
                    dlgt->output(consoleIsActive ? "console ON.\n" : "console OFF.\n");
                    dlgt->setConsoleState(consoleIsActive, MenuIsActive);
                }
            }
            if (consoleIsActive){
                if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE){
                    dlgt->backspaceCommand();
                }
                else if(sdlEvent.key.keysym.sym == SDLK_RETURN){
                    std::string command = dlgt->submitCommand();
                    parseCommand(command);
                }
                else if(sdlEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL){
                    SDL_SetClipboardText(dlgt->getLastConsoleLine().c_str());
                }
                else if(sdlEvent.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL){
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

void System_UserControl::parseCommand(std::string command){
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::vector<std::string> args;
    std::string temp;
    while (commandLine >> temp){
        args.push_back(temp);
        temp.clear();
    }
    if (MenuIsActive){
        if (args[0] == "exit"){
            dlgt->quit();
        } else if (args[0] == "new"){
            dlgt->output("new game command not yet implemented\n");
        } else if (args[0] == "load"){
            dlgt->output("load game command not yet implemented\n");
        } else if (args[0] == "save"){
            dlgt->output("save game command not yet implemented\n");
        } else if (args[0] == "help"){
            dlgt->output("You're in the menu. Type one of the following: 'new', 'load', 'save', or 'exit'.\n");
        } else if (args[0] == "add" || args[0] == "delete"){
            dlgt->output("That command isn't available from the menu (ESC to exit menu).\n");
        } else {
            std::string error = args[0] + " is not a valid menu option.\n";
            dlgt->output(error.c_str());
        }
    } else {
        if (args[0] == "add"){
            dlgt->output("Command not yet implemented.\n");
        } else if (args[0] == "delete"){
            dlgt->output("Command not yet implemented.\n");
        } else if (args[0] == "debug"){
            DU_ID boxID = bank->createBox("box", 0,0,0, 0,0,0, 0,0,0, 0,0,0);
            std::string output = bank->getName(boxID) + "\n" + bank->listComponents(boxID) + "\n";
            dlgt->output(output.c_str());
        } else if (args[0] == "help"){
            dlgt->output("You're in the console. You have access to the following commands:\n\tadd [component type] [entity ID] [component parameters...]\n\tdelete [component type] [entity ID]\n");
        } else if (args[0] == "new" || args[0] == "load" || args[0] == "save" || args[0] == "exit"){
            dlgt->output("Menu commands only available through menu (ESC to enter menu).\n");
        } else {
            std::string error = args[0] + " is not a valid command.\n";
            dlgt->output(error.c_str());
        }
    }
}

void System_UserControl::presentTextMenu(){
    
}

void System_UserControl::handleControlKeys(const Uint8* keyStates){
    
//    if(keyStates[SDL_SCANCODE_W]){
//        //do w thing.
//    }
}

//</editor-fold>