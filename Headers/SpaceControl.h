/* 
 * File:   SpaceControl.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef SPACECONTROL_H
#define	SPACECONTROL_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    namespace ControlSS{
        enum inputDirections{
            FORWARD  = 0,
            BACKWARD = 1,
            LEFT     = 2,
            RIGHT    = 3,
            UP       = 4,
            DOWN     = 5,
            ROLL     = 6,
            PITCH    = 7,
            YAW      = 8,
            BREAK    = 9
        };
    }
    
    struct SpaceControl : public Component
    {
        SpaceControl(const DUA_dbl& fw, const DUA_dbl& bk, const DUA_dbl& lf, const DUA_dbl& rt, const DUA_dbl& up, const DUA_dbl& dn, const DUA_dbl& roll, const DUA_dbl& pitch, const DUA_dbl& yaw){
            thrust[ControlSS::FORWARD]  = fw;
            thrust[ControlSS::BACKWARD] = bk;
            thrust[ControlSS::LEFT]     = lf;
            thrust[ControlSS::RIGHT]    = rt;
            thrust[ControlSS::UP]       = up;
            thrust[ControlSS::DOWN]     = dn;
            thrust[ControlSS::ROLL]     = roll;
            thrust[ControlSS::PITCH]    = pitch;
            thrust[ControlSS::YAW]      = yaw;
            
        }
        inline void transform(const glm::mat4& transform){
            forward = transform * forwardOrig;
            right = transform * rightOrig;
            up = transform * upOrig;
        }
        inline void zeroInputs(){
            memset(inputs,0,sizeof(inputs));
        }
        inline void applyInput(const int whichInput, const DUA_dbl& value){
            inputs[whichInput] += value;
        }
        glm::vec4 forwardOrig = {0, 0, -1, 1};
        glm::vec4 rightOrig = {1, 0, 0, 1};
        glm::vec4 upOrig = {0, 1, 0, 1};
        
        glm::vec4 forward = forwardOrig;
        glm::vec4 right = rightOrig;
        glm::vec4 up = upOrig;
        
        // forward, backward, left, right, up, down roll, pitch, yaw, break
        DUA_dbl inputs[10] = {0};
        DUA_dbl thrust[9] = {1};  // lacks break, which is just scalar 0-1 value and uses the 6 directional thrusts.        
        
    };

}

#endif

