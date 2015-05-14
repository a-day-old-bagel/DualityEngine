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
        PositionVeloc(const DU_FLOAT &velX, const DU_FLOAT &velY, const DU_FLOAT &velZ){
            velLinear = {velX, velY, velZ};
        }
        DU_VEC3 velLinear;
    };

}

#endif	/* POSITIONVELOC_H */

