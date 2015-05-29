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
                    dlgt->setConsoleState(consoleIsActive, MenuIsActive);
                }
            }
            if (consoleIsActive){
                if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE){
                    dlgt->backspaceCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_DELETE){
                    dlgt->deleteCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_RETURN){
                    std::string command = dlgt->submitCommand();
                    parseCommand(command);
                } else if(sdlEvent.key.keysym.sym == SDLK_UP){
                    dlgt->upOneCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_DOWN){
                    dlgt->downOneCommand();
                } else if(sdlEvent.key.keysym.sym == SDLK_LEFT){
                    dlgt->leftCursor();
                } else if(sdlEvent.key.keysym.sym == SDLK_RIGHT){
                    dlgt->rightCursor();
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

void System_UserControl::parseCommand(std::string command){
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::vector<std::string> args;
    std::string temp;
    while (commandLine >> temp){
        args.push_back(temp);
        temp.clear();
    }
    const int numArgs = args.size();
    
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
            std::string error = "Not a menu option: " + args[0];
            dlgt->output(error.c_str());
        }
    } else {
        if (args[0] == "newent"){
            if (numArgs == 2){
                dlgt->output("newent command not yet implemented in the console.\n");
            } else {
                handleBadUsage(args[0]);
            }
        }else if (args[0] == "add"){
            if (numArgs > 1){
                if (componentArgs.count(args[1])){
                    dlgt->outputStr("Adding " + args[1] + " components not yet available from console.\n");
                } else {
                    dlgt->outputStr("Unknown component: " + args[1] + "\n");
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "delete"){
            if (numArgs == 3){
                if (componentArgs.count(args[1])){
                    dlgt->outputStr("Deleting " + args[1] + " components not yet available from console.\n");
                } else {
                    dlgt->outputStr("Unknown component: " + args[1] + "\n");
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "debug"){
            DUA_id boxID = bank->createBox("box", 0,0,0, 0,0,0, 0,0,0, 0,0,0);
            dlgt->outputStr(bank->getName(boxID) + "\n" + bank->listComponents(boxID) + "\n");
        } else if (args[0] == "help"){
            if (numArgs == 1){
                dlgt->outputStr("You're in the console. You have access to the following commands:\n    "
                             + commandUsages["newent"]  + "\n    "
                             + commandUsages["add"]     + "\n    "
                             + commandUsages["delete"]  + "\n    "
                             + commandUsages["help"]);
            } else if (numArgs == 2){
                if (commandHelps.count(args[1])){
                    dlgt->outputStr("The " + args[1] + " command " + commandHelps[args[1]] + "\n" +
                                 "Usage is " + commandUsages[args[1]] + "\n" +
                                 "Example: " + commandExamples[args[1]] + "\n");
                }else if (componentHelps.count(args[1])){
                    dlgt->outputStr("The " + args[1] + " component " + componentHelps[args[1]] + "\n" +
                                 "Its arguments are: " + componentArgs[args[1]] + "\n");
                }else {
                    dlgt->outputStr("No documentation for: " + args[1] + "\n");
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "new" || args[0] == "load" || args[0] == "save" || args[0] == "exit"){
            dlgt->output("Menu commands only available through menu (ESC to enter menu).\n");
        } else {
            dlgt->outputStr("Bad command: " + args[0] + "\nType \"help\" for a list of commands.\n");
        }
    }
}

void System_UserControl::handleBadUsage(std::string command){
    dlgt->outputStr("Incorrect usage of " + command + ". Try \"" + commandUsages[command] +
                 "\"\n    for more help with the " + command + " command, type \"help " + command + "\"\n");
}

void System_UserControl::presentTextMenu(){
    
}

void System_UserControl::handleControlKeys(const Uint8* keyStates){
    
//    if(keyStates[SDL_SCANCODE_W]){
//        //do w thing.
//    }
}

//</editor-fold>