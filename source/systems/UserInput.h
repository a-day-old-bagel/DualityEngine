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
        bool consoleIsActive;
        bool MenuIsActive;
        static const std::string menuText;

        void parseMenuCommand(const std::string& command);
        void handleKeyDown(SDL_Event& event);
        void handleControlKeys(const Uint8* keyStates);
        void handleMouseMotion(int x, int y);
        void keyPressed_backQuote();
        void keyPressed_esc();
    public:
        System_UserInput(Bank * bank);
        ~System_UserInput();
        void onTick();
        bool onInit(std::stringstream& output);
        void onClean();
    };

}

#endif	/* USERCONTROL_H */

