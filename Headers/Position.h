/* 
 * File:   Spatial.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef POSITION_H
#define	POSITION_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct Position
    {
        Position(const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ){
            position = {posX, posY, posZ};
        }
        DUA_vec3 position;
    };

}

#endif	/* POSITION_H */

