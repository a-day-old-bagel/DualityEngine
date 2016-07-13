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

#include "controlTypes.h"

#ifdef _WIN32
typedef unsigned int uint;
#endif


namespace DualityEngine {

    namespace ControlSS{
        enum inputDirections{
            FORWARD  =  0,
            BACKWARD =  1,
            RIGHT    =  2,
            LEFT     =  3,
            UP       =  4,
            DOWN     =  5,
            PITCHPOS =  6,
            PITCHNEG =  7,
            YAWPOS   =  8,
            YAWNEG   =  9,
            ROLLPOS  =  10,
            ROLLNEG  =  11,
            LINBRAKE =  12,
            ANGBRAKE =  13,
        };
    }
    
    struct SpaceControl
    {
    	SpaceControl(const DUA_float& fw, const DUA_float& bk,
                const DUA_float& lf, const DUA_float& rt,
                const DUA_float& up, const DUA_float& dn,
                const DUA_float& pitchp, const DUA_float& pitchn,
                const DUA_float& yawp,   const DUA_float& yawn,
                const DUA_float& rollp,  const DUA_float& rolln)
					: thrust{fw,bk,rt,lf,up,dn,pitchp,pitchn,yawp,yawn,rollp,rolln} {

		}
        inline void transform(const glm::mat4& transMat){
            transform(glm::mat3(transMat));
        }
        inline void transform(const glm::mat3& transMat){
            for (uint i = 0; i < 3; ++i){
                currentAxes[i] = transMat * originalAxes[i];
            }

        }
        inline void zeroInputs(){
            for (uint i = 0; i < 12; ++i){
                throttle[i] = 0;
            }
            throttle[ControlSS::LINBRAKE] = autoBrakeLinear;
            throttle[ControlSS::ANGBRAKE] = autoBrakeAngular;
        }
        inline void applyInput(const int whichInput, const DUA_float& value){
            throttle[whichInput] += value;
        }
        
        glm::vec3 originalAxes[3] = {{0, 0, -1}, {1, 0, 0}, {0, 1, 0}};
        glm::vec3 currentAxes[3] = {{0, 0, -1}, {1, 0, 0}, {0, 1, 0}};
        
        // forward, backward, right, left, up, down roll, pitch, yaw, linear break, angular break
        // all input values range from 0 - 1.
        DUA_float throttle[14] = {0};
        DUA_float thrust[12] = {1.0};  // lacks brakes, which use the 6 directional thrusts and 6 angular torques respectively.
        bool autoBrakeLinear = true;
        bool autoBrakeAngular = true;

        uint32_t angBrakeEngageCounter = 0;

        // These modify the sensitivity of the gyro controls (so mouse sensetivity, probably)
        DUA_float rotSense = 1.f;
        DUA_float rotSenseMod = 1.f;
        
    };

}

#endif

