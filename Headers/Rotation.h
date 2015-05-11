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
        Rotation(const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ){
            rotation = {rotX, rotY, rotZ};
        }
        glm::vec3 rotation;
    };

}

#endif	/* ROTATION_H */

