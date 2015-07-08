/* 
 * File:   AmbientLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:33 PM
 */

#ifndef AMBIENTLIGHT_H
#define	AMBIENTLIGHT_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct AmbientLight : public Component
    {
        AmbientLight(const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue){
            color = {red, green, blue};
        }
        glm::vec3 color;
    };

}

#endif	/* AMBIENTLIGHT_H */

