/* 
 * File:   PointLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:32 PM
 */

#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct PointLight
    {
        PointLight(const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue, const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ){
            color = {red, green, blue};
            position = {posX, posY, posZ};
        }
        glm::vec3 color;
        glm::vec3 position;
    };

}

#endif	/* POINTLIGHT_H */

