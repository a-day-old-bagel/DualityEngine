/* 
 * File:   AmbientLight.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 8:33 PM
 */

#ifndef AMBIENTLIGHT_H
#define	AMBIENTLIGHT_H

#include <glm/glm.hpp>
#include "Component.h"

struct AmbientLight : Component
{
    glm::vec3 color = {50, 50, 50};
};

#endif	/* AMBIENTLIGHT_H */

