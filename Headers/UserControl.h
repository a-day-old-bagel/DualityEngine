/*******************************************************************************
 * File:   UserControl.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:38 AM
 * 
 * 
 ******************************************************************************/

#ifndef USERCONTROL_H
#define	USERCONTROL_H

#include <SDL.h>
#include "System.h"
//#include <glm/glm.hpp>
#include "ControlDelegates.h"

namespace DualityEngine {

    class System_UserControl : public System
    {
    private:
        ControlDelegates* dlgt;
        SDL_Event sdlEvent;
        bool consoleIsActive = false;
        bool MenuIsActive = false;
        DUA_id localActiveControl = DUA_NULL_ID;
        Control* pControlCurrent;
        Control* pDummyControl;
        Position* pPositionCurrent;
        Position* pDummyPosition;
        Orientation* pOrientationCurrent;
        Orientation* pDummyOrientation;
        const std::string menuText = "****************************************\n*  MENU - ENTER ONE OF THESE OPTIONS:  *\n*        new, load, save, exit         *\n****************************************\n";
        void handleMenuCommand(const std::string& command);
        void handleControlKeys(const Uint8* keyStates);
        void checkActiveControl();
    public:
        System_UserControl(ComponentBank* bank, ControlDelegates* delegates);
        ~System_UserControl();
        void tick() override;
        bool init(std::stringstream& output) override;
        void clean() override;
    };

}

#endif	/* USERCONTROL_H */

