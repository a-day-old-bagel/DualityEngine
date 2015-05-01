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
        Spatial(const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ, const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ){
            position = {posX, posY, posZ};
            rotation = {rotX, rotY, rotZ};
        }
        glm::vec3 position;
        glm::vec3 rotation;
    };

}

#endif	/* SPATIAL_H */

