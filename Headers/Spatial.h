/* 
 * File:   Spatial.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef SPATIAL_H
#define	SPATIAL_H

#include <glm/glm.hpp>
#include "Component.h"

struct Spatial : Component
{
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
};

#endif	/* SPATIAL_H */

