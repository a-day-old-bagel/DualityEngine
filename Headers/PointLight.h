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
        PointLight(const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue, const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ){
            color = {red, green, blue};
            position = {posX, posY, posZ};
        }
        DU_VEC3 color;
        DU_VEC3 position;
    };

}

#endif	/* POINTLIGHT_H */

