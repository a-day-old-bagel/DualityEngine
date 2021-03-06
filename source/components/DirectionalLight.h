/* 
 * File:   DirectionalLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:33 PM
 */

#ifndef DIRECTIONALLIGHT_H
#define	DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct DirectionalLight : public Component
    {
        DirectionalLight(const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue, const DUA_dbl &dirX, const DUA_dbl &dirY, const DUA_dbl &dirZ){
            color = {red, green, blue};
            direction = {dirX, dirY, dirZ};
        }
        glm::vec3 color;
        glm::vec3 direction;
    };

}

#endif	/* DIRECTIONALLIGHT_H */

