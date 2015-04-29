/* 
 * File:   Motion.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef MOTION_H
#define	MOTION_H

#include <glm/glm.hpp>

namespace DualityEngine {

    struct Motion
    {
        glm::vec3 velRegular;
        glm::vec3 velAngular;
    };

}

#endif	/* MOTION_H */

