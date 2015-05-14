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
        AmbientLight(const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue){
            color = {red, green, blue};
        }
        DU_VEC3 color;
    };

}

#endif	/* AMBIENTLIGHT_H */

