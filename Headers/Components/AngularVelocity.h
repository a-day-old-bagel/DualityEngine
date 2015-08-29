/*******************************************************************************
 * File:   RotationVeloc.h
 * Author: Galen Cochrane *
 * Created on May 10, 2015, 2:50 PM
 * 
 * 
 ******************************************************************************/

#ifndef ROTATIONVELOC_H
#define	ROTATIONVELOC_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct AngularVelocity : public Component
    {
        AngularVelocity(const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ){
            velAngular = {rotX, rotY, rotZ};
        }
        inline void applyImpulse(const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ){
            velAngular.x += rotX;
            velAngular.y += rotY;
            velAngular.z += rotZ;
        }
        inline void applyImpulse(glm::vec4 impulse){
            applyImpulse(impulse.x, impulse.y, impulse.z);
        }
        inline void applyImpulse(glm::vec3 impulse){
            applyImpulse(impulse.x, impulse.y, impulse.z);
        }
        inline void zero(){
            velAngular.x = 0;
            velAngular.y = 0;
            velAngular.z = 0;
        }
        glm::vec3 velAngular;
    };

}

#endif	/* ROTATIONVELOC_H */

