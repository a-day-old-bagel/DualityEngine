/* 
 * File:   Motion.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef MOTION_H
#define	MOTION_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct Motion
    {
        Motion(const FLOAT &velX, const FLOAT &velY, const FLOAT &velZ, const FLOAT &anvX, const FLOAT &anvY, const FLOAT &anvZ){
            velLinear = {velX, velY, velZ};
            velAngular = {anvX, anvY, anvZ};
        }
        glm::vec3 velLinear;
        glm::vec3 velAngular;
    };

}

#endif	/* MOTION_H */

