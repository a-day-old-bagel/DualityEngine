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

    void rewriteAsZero(DualityEngine::Mat4x4& mat);
    void rewriteAsIdentity(DualityEngine::Mat4x4& mat);
    void rewriteAsScale(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void rewriteAsUniformScale(DualityEngine::Mat4x4& mat, DUA_matrixVal U);
    void rewriteAsTranslation(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void rewriteAsRotationWorldAxisZyx(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void rewriteAsRotationWorldAxisZyx(DualityEngine::Mat4x4& mat, DualityEngine::Vec3& rotation);
    void rewriteAsRotationArbitraryAxis(DualityEngine::Mat4x4 & mat, DualityEngine::Vec4& unitAxis, DUA_matrixVal angle);
    void rewriteAsViewFree(DualityEngine::Mat4x4& mat, DualityEngine::Vec4& center, DualityEngine::Vec4* axes);
    void rewriteAsViewFps(DualityEngine::Mat4x4& mat, DualityEngine::Vec4& center, DUA_matrixVal pitch,
                          DUA_matrixVal yaw);
    void rewriteAsProjection(DualityEngine::Mat4x4& mat, DUA_matrixVal FOV, DUA_matrixVal aspRat, DUA_matrixVal zNear,
                             DUA_matrixVal zFar);
    void transformMatrixTranslate(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);
    void transformMatrixScale(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z);

} // namespace DualityEngine


#endif	/* DUA_MATHFUNCTIONS_H */

