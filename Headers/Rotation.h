/*******************************************************************************
 * File:   Rotation.h
 * Author: Galen Cochrane *
 * Created on May 10, 2015, 2:05 PM
 * 
 * 
 ******************************************************************************/

#ifndef ROTATION_H
#define	ROTATION_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct Rotation
    {
        Rotation(const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ){
            rotation = {rotX, rotY, rotZ};
        }
        DUA_vec3 rotation;
    };

}

#endif	/* ROTATION_H */

