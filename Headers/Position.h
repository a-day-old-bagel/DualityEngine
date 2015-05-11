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
        Position(const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ){
            position = {posX, posY, posZ};
        }
        glm::vec3 position;
    };

}

#endif	/* POSITION_H */

