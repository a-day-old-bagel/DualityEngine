/* 
 * File:   DirectionalLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:33 PM
 */

#ifndef DIRECTIONALLIGHT_H
#define	DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include "Component.h"

struct DirectionalLight : Component
{
    glm::vec3 color = {50, 50, 50};
    glm::vec3 rotation = {0, 1, 0};
};

#endif	/* DIRECTIONALLIGHT_H */

