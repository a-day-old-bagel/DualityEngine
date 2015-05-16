/* 
 * File:   DirectionalLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:33 PM
 */

#ifndef DIRECTIONALLIGHT_H
#define	DIRECTIONALLIGHT_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct DirectionalLight
    {
        DirectionalLight(const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue, const DUA_DBL &dirX, const DUA_DBL &dirY, const DUA_DBL &dirZ){
            color = {red, green, blue};
            direction = {dirX, dirY, dirZ};
        }
        DUA_VEC3 color;
        DUA_VEC3 direction;
    };

}

#endif	/* DIRECTIONALLIGHT_H */

