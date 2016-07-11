/*******************************************************************************
 * File:   Orientation.h
 * Author: Galen Cochrane *
 * Created on May 10, 2015, 2:05 PM
 * 
 * 
 ******************************************************************************/

#ifndef ORIENTATION_H
#define	ORIENTATION_H

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Component.h"

namespace DualityEngine {

    struct Orientation : public Component
    {
        Orientation(const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ){
            orientation = {rotX, rotY, rotZ};
            matrixIsCurrent = false;
        }
        inline void updateMatrix(){
            transform = glm::eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
            matrixIsCurrent = true;
        }
        inline glm::mat4 getMatrix(){
            if (!matrixIsCurrent){
                updateMatrix();
            }
            return transform;
        }
        inline void rotate(glm::vec3 rotation){
            orientation += rotation;
            matrixIsCurrent = false;
        }
        DUA_vec3 orientation;
        glm::mat4 transform;    // TOTAL sum including parents
        bool matrixIsCurrent;
    };

}

#endif	/* ORIENTATION_H */

