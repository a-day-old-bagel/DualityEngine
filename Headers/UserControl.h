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
#include <glm/glm.hpp>
#include "ControlDelegateBag.h"

namespace DualityEngine {

    class System_UserControl : public System
    {
    private:
        DelegateBag* delegates;
        SDL_Event sdlEvent;
    public:
        System_UserControl(ComponentBank* bank, DelegateBag* delegates);
        ~System_UserControl();
        void tick() override;
        bool init(std::stringstream& output) override;
    };

}

#endif	/* USERCONTROL_H */

