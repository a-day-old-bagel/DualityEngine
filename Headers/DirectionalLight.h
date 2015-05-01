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
        DirectionalLight(const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue, const FLOAT &dirX, const FLOAT &dirY, const FLOAT &dirZ){
            color = {red, green, blue};
            direction = {dirX, dirY, dirZ};
        }
        glm::vec3 color;
        glm::vec3 direction;
    };

}

#endif	/* DIRECTIONALLIGHT_H */

