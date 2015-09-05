/*******************************************************************************
 * File:   UserInput.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:38 AM
 * 
 * 
 ******************************************************************************/

#ifndef USERINPUT_H
#define	USERINPUT_H

#include <SDL.h>
#include "System.h"

namespace DualityEngine {

    class System_UserInput : public System<System_UserInput>
    {
    private:
        SDL_Event sdlEvent;   
        bool consoleIsActive = false;
        bool MenuIsActive = false;
        const std::string menuText = "****************************************\n*  MENU - ENTER ONE OF THESE OPTIONS:  *\n*     new, load, save, exit, help      *\n****************************************\n";
        void parseMenuCommand(const std::string& command);
        void handleKeyDown(SDL_Event& event);
        void handleControlKeys(const Uint8* keyStates);
        void handleMouseMotion(int x, int y);
        void keyPressed_backQuote();
        void keyPressed_esc();
    public:
        System_UserInput(ComponentBank* bank);
        ~System_UserInput();
        void tick();
        bool init(std::stringstream& output);
    };

}

#endif	/* USERCONTROL_H */

