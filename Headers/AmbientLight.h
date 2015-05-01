/* 
 * File:   AmbientLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:33 PM
 */

#ifndef AMBIENTLIGHT_H
#define	AMBIENTLIGHT_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct AmbientLight
    {
        AmbientLight(const colorByte &red, const colorByte &green, const colorByte &blue){
            color = {red, green, blue};
        }
        glm::vec3 color;
    };

}

#endif	/* AMBIENTLIGHT_H */

