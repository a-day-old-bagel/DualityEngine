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

namespace DualityEngine{


    glm::vec3 operator * (glm::vec3 vec, float scalar);


    DUA_matrixVal dotVec4(DualityEngine::Vec4& mat1, DualityEngine::Vec4& mat2);
    DUA_matrixVal dotVec3(DualityEngine::Vec3& mat1, DualityEngine::Vec3& mat2);

    void rewriteAsZeroMatrix(DualityEngine::Mat4x4& mat);
    void rewriteAsIdentityMatrix(DualityEngine::Mat4x4& mat);
    void rewriteAsScaleMatrix(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void rewriteAsUniformScaleMatrix(DualityEngine::Mat4x4& mat, DUA_matrixVal U);
    void rewriteAsTranslationMatrix(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void rewriteAsRotationMatrixWorldAxisZyx(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void rewriteAsRotationMatrixWorldAxisZyx(DualityEngine::Mat4x4& mat, DualityEngine::Vec3& rotation);
    void rewriteAsRotationMatrixArbitraryAxis(DualityEngine::Mat4x4& mat, DualityEngine::Vec4& unitAxis, DUA_matrixVal angle);
    void rewriteAsViewMatrixFree(DualityEngine::Mat4x4& mat, DualityEngine::Vec4& center, DualityEngine::Vec4* axes);
    void rewriteAsViewMatrixFps(DualityEngine::Mat4x4& mat, DualityEngine::Vec4& center, DUA_matrixVal pitch, DUA_matrixVal yaw);
    void rewriteAsProjectionMatrix(DualityEngine::Mat4x4& mat, DUA_matrixVal FOV, DUA_matrixVal aspRat, DUA_matrixVal zNear, DUA_matrixVal zFar);

    void transformMatrixTranslate(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void transformMatrixScale(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);

}


#endif	/* DUA_MATHFUNCTIONS_H */

