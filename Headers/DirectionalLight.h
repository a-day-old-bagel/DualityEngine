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
        DirectionalLight(const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue, const DU_FLOAT &dirX, const DU_FLOAT &dirY, const DU_FLOAT &dirZ){
            color = {red, green, blue};
            direction = {dirX, dirY, dirZ};
        }
        DU_VEC3 color;
        DU_VEC3 direction;
    };

}

#endif	/* DIRECTIONALLIGHT_H */

