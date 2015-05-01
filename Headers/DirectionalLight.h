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
        DirectionalLight(const colorByte &red, const colorByte &green, const colorByte &blue, const double &dirX, const double &dirY, const double &dirZ){
            color = {red, green, blue};
            direction = {dirX, dirY, dirZ};
        }
        glm::vec3 color;
        glm::vec3 direction;
    };

}

#endif	/* DIRECTIONALLIGHT_H */

