/* 
 * File:   Spatial.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef SPATIAL_H
#define	SPATIAL_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct Spatial
    {
        Spatial(const double &posX, const double &posY, const double &posZ, const double &rotX, const double &rotY, const double &rotZ){
            position = {posX, posY, posZ};
            rotation = {rotX, rotY, rotZ};
        }
        glm::vec3 position;
        glm::vec3 rotation;
    };

}

#endif	/* SPATIAL_H */

