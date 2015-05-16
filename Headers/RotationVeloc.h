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
        RotationVeloc(const DUA_DBL &rotX, const DUA_DBL &rotY, const DUA_DBL &rotZ){
            velAngular = {rotX, rotY, rotZ};
        }
        DUA_VEC3 velAngular;
    };

}

#endif	/* ROTATIONVELOC_H */

