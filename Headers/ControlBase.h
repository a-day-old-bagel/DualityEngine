/* 
 * File:   ControlBase.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef CONTROLBASE_H
#define	CONTROLBASE_H

#include <cstring>
#include <SDL.h>
#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {
    
    namespace ControlTypes{
        enum type{
            NONE = 0,
            SPACE = 1
        };
    }
    
    struct ControlBase : public Component
    {
        ControlBase(int type){
            this->type = type;
        }
        Uint8* keyStates = NULL;
        int type = ControlTypes::NONE;
    };

}

#endif

