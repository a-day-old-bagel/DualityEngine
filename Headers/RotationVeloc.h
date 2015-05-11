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

namespace DualityEngine {

    struct RotationVeloc
    {
        RotationVeloc(const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ){
            velAngular = {rotX, rotY, rotZ};
        }
        glm::vec3 velAngular;
    };

}

#endif	/* ROTATIONVELOC_H */

