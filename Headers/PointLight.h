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
        PointLight(const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue, const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ){
            color = {red, green, blue};
            position = {posX, posY, posZ};
        }
        DUA_VEC3 color;
        DUA_VEC3 position;
    };

}

#endif	/* POINTLIGHT_H */

