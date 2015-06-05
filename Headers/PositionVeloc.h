/* 
 * File:   Motion.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef POSITIONVELOC_H
#define	POSITIONVELOC_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct LinearVelocity : public Component
    {
        LinearVelocity(const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ){
            velLinear = {velX, velY, velZ};
        }
        DUA_vec3 velLinear;
    };

}

#endif	/* POSITIONVELOC_H */

