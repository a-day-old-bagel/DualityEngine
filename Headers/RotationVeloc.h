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
        DUA_vec3 velAngular;
    };

}

#endif	/* ROTATIONVELOC_H */

