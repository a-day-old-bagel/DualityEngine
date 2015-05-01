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
        Motion(const double &velX, const double &velY, const double &velZ, const double &anvX, const double &anvY, const double &anvZ){
            velLinear = {velX, velY, velZ};
            velAngular = {anvX, anvY, anvZ};
        }
        glm::vec3 velLinear;
        glm::vec3 velAngular;
    };

}

#endif	/* MOTION_H */

