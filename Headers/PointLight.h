/* 
 * File:   PointLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:32 PM
 */

#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct PointLight : public Component
    {
        PointLight(const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue, const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ){
            color = {red, green, blue};
            position = {posX, posY, posZ};
        }
        DUA_vec3 color;
        DUA_vec3 position;
    };

}

#endif	/* POINTLIGHT_H */

