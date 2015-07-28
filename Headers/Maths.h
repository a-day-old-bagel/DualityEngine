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
#include "Matrix.h"

glm::vec3 operator * (glm::vec3 vec, float scalar);

void makeProjectionMatrixD(DualityEngine::Mat4x4d& mat);
void makeProjectionMatrixF(DualityEngine::Mat4x4f& mat);

void makeRotationMatrixArbitraryAxisD(DualityEngine::Mat4x4d& mat, DualityEngine::Vec4d& unitAxis, double angle);
void makeRotationMatrixArbitraryAxisF(DualityEngine::Mat4x4f& mat, DualityEngine::Vec4f& unitAxis, float angle);

#endif	/* MATHS_H */

