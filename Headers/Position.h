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
        Position(const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ){
            position = {posX, posY, posZ};
        }
        DUA_VEC3 position;
    };

}

#endif	/* POSITION_H */

