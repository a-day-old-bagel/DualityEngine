/* 
 * File:   Motion.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef POSITIONVELOC_H
#define	POSITIONVELOC_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct PositionVeloc
    {
        PositionVeloc(const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ){
            velLinear = {velX, velY, velZ};
        }
        DUA_vec3 velLinear;
    };

}

#endif	/* POSITIONVELOC_H */

