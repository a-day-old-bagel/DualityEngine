/* 
 * File:   PointLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:32 PM
 */

#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include <glm/glm.hpp>
#include "Component.h"

struct PointLight : Component
{
    glm::vec3 color;
    glm::vec3 position;
};

#endif	/* POINTLIGHT_H */

