/*******************************************************************************
 * File:   Functions.h
 * Author: Galen Cochrane *
 * Created on July 9, 2015, 12:32 AM
 * 
 * 
 ******************************************************************************/

#ifndef DUA_MATHFUNCTIONS_H
#define	DUA_MATHFUNCTIONS_H

#include <glm/glm.hpp>
#include "Matrix.h"

glm::vec3 operator * (glm::vec3 vec, float scalar);

double dotVec4D(DualityEngine::Vec4d& mat1, DualityEngine::Vec4d& mat2);
float dotVec4F(DualityEngine::Vec4f& mat1, DualityEngine::Vec4f& mat2);
double dotVec3D(DualityEngine::Vec3d& mat1, DualityEngine::Vec3d& mat2);
float dotVec3F(DualityEngine::Vec3f& mat1, DualityEngine::Vec3f& mat2);

void convertToZeroMatrixD(DualityEngine::Mat4x4d& mat);
void convertToZeroMatrixF(DualityEngine::Mat4x4f& mat);

void convertToIdentityMatrixD(DualityEngine::Mat4x4d& mat);
void convertToIdentityMatrixF(DualityEngine::Mat4x4f& mat);

void convertToScaleMatrixD(DualityEngine::Mat4x4d& mat, double X, double Y, double Z);
void convertToScaleMatrixF(DualityEngine::Mat4x4f& mat, float X, float Y, float Z);

void convertToUniformScaleMatrixD(DualityEngine::Mat4x4d& mat, double U);
void convertToUniformScaleMatrixF(DualityEngine::Mat4x4f& mat, float U);

void convertToTranslationMatrixD(DualityEngine::Mat4x4d& mat, double X, double Y, double Z);
void convertToTranslationMatrixF(DualityEngine::Mat4x4f& mat, float X, float Y, float Z);

void convertToRotationMatrixWorldAxisZyxD(DualityEngine::Mat4x4d& mat, double X, double Y, double Z);
void convertToRotationMatrixWorldAxisZyxF(DualityEngine::Mat4x4f& mat, float X, float Y, float Z);
void convertToRotationMatrixWorldAxisZyxD(DualityEngine::Mat4x4d& mat, DualityEngine::Vec3d& rotation);
void convertToRotationMatrixWorldAxisZyxF(DualityEngine::Mat4x4f& mat, DualityEngine::Vec3f& rotation);

void convertToRotationMatrixArbitraryAxisD(DualityEngine::Mat4x4d& mat, DualityEngine::Vec4d& unitAxis, double angle);
void convertToRotationMatrixArbitraryAxisF(DualityEngine::Mat4x4f& mat, DualityEngine::Vec4f& unitAxis, float angle);

void convertToViewMatrixFreeD(DualityEngine::Mat4x4d& mat, DualityEngine::Vec4d& center, DualityEngine::Vec4d* axes);
void convertToViewMatrixFreeF(DualityEngine::Mat4x4f& mat, DualityEngine::Vec4f& center, DualityEngine::Vec4f* axes);
void convertToViewMatrixFpsD(DualityEngine::Mat4x4d& mat, DualityEngine::Vec4d& center, double pitch, double yaw);
void convertToViewMatrixFpsF(DualityEngine::Mat4x4f& mat, DualityEngine::Vec4f& center, float pitch, float yaw);

void convertToProjectionMatrixD(DualityEngine::Mat4x4d& mat, double FOV, double aspRat, double zNear, double zFar);
void convertToProjectionMatrixF(DualityEngine::Mat4x4f& mat, float FOV, float aspRat, float zNear, float zFar);





#endif	/* DUA_MATHFUNCTIONS_H */

