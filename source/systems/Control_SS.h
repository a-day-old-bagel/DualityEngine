/* 
 * File:   Control_SS.h
 * Author: adayoldbagel
 *
 * Created on February 11, 2015, 11:26 PM
 */

#ifndef CONTROL_SS_H
#define	CONTROL_SS_H

#include "System.h"
#include <glm/glm.hpp>

namespace DualityEngine {

    class System_Control_SS : public System<System_Control_SS>
    {  
    public:
        System_Control_SS(Bank * bank);
        void onTick();
        bool onInit(std::stringstream& output);
        void onClean();
    };

}

#endif

