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

    class System_Control_SS : public System
    {  
    public:
        System_Control_SS(ComponentBank* bank);
        ~System_Control_SS() override;
        void tick() override;
        bool init(std::stringstream& output) override;
    };

}

#endif
