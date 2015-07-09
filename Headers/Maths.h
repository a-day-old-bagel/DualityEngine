/*******************************************************************************
 * File:   Maths.h
 * Author: Galen Cochrane *
 * Created on July 9, 2015, 12:32 AM
 * 
 * 
 ******************************************************************************/

#ifndef MATHS_H
#define	MATHS_H

#include <glm/glm.hpp>

glm::vec3 operator * (glm::vec3 vec, float scalar){
    return glm::vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

#endif	/* MATHS_H */

