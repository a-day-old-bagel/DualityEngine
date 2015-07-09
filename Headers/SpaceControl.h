/* 
 * File:   SpaceControl.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef SPACECONTROL_H
#define	SPACECONTROL_H

#include <cstring>
#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    namespace ControlSS{
        enum inputDirections{
            FORWARD  = 0,
            BACKWARD = 1,
            RIGHT    = 2,
            LEFT     = 3,
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
        inline void transform(const glm::mat4& transMat){
            transform(glm::mat3(transMat));
        }
        inline void transform(const glm::mat3& transMat){
            currentAxes[0] = transMat * forwardOrig;
            currentAxes[1] = transMat * rightOrig;
            currentAxes[2] = transMat * upOrig;

        }
        inline void zeroInputs(){
            for (uint i = 0; i < 10; ++i){
                inputs[i] = 0;
            }
        }
        inline void applyInput(const int whichInput, const DUA_float& value){
            inputs[whichInput] += value;
        }
        glm::vec3 forwardOrig = {0, 0, -1};
        glm::vec3 rightOrig = {1, 0, 0};
        glm::vec3 upOrig = {0, 1, 0};
        
        glm::vec3 currentAxes[3];
        
        // forward, backward, right, left, up, down roll, pitch, yaw, break
        // all input values range from 0 - 1.
        DUA_float inputs[10] = {0};
        DUA_float thrust[9] = {1.0};  // lacks break, which uses the 6 directional thrusts.        
        
    };

}

#endif

