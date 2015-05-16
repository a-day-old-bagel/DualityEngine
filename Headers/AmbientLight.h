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
        AmbientLight(const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue){
            color = {red, green, blue};
        }
        DUA_VEC3 color;
    };

}

#endif	/* AMBIENTLIGHT_H */

