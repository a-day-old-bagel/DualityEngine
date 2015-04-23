/* 
 * File:   Motion.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef MOTION_H
#define	MOTION_H

#include <glm/glm.hpp>
#include "Component.h"

struct Motion : Component
{
    glm::vec3 velRegular = {0, 0, 0};
    glm::vec3 velAngular = {0, 0, 0};
};

#endif	/* MOTION_H */

