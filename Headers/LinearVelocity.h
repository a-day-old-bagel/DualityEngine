/* 
 * File:   Motion.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef POSITIONVELOC_H
#define	POSITIONVELOC_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct LinearVelocity : public Component
    {
        LinearVelocity(const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ){
            velLinear = {velX, velY, velZ};
        }
        inline void applyImpulse(const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ){
            velLinear.x += velX;
            velLinear.y += velY;
            velLinear.z += velZ;
        }
        inline void applyImpulse(const glm::vec3 &impulse){
            applyImpulse(impulse.x, impulse.y, impulse.z);
        }
        inline void applyImpulse(const glm::vec4 &impulse){
            applyImpulse(impulse.x, impulse.y, impulse.z);
        }
        glm::vec3 velLinear;
    };

}

#endif	/* POSITIONVELOC_H */

