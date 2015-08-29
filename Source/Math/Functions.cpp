#include "Functions.h"
#include <cmath>

namespace DualityEngine{

    glm::vec3 operator * (glm::vec3 vec, float scalar){
        return glm::vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }




    DUA_matrixVal dotVec4(DualityEngine::Vec4& mat1, DualityEngine::Vec4& mat2){
        return mat1(0,0) * mat2(0,0) + mat1(1,0) * mat2(1,0) + mat1(2,0) * mat2(2,0);
    }

    DUA_matrixVal dotVec3(DualityEngine::Vec3& mat1, DualityEngine::Vec3& mat2){
        return mat1(0,0) * mat2(0,0) + mat1(1,0) * mat2(1,0) + mat1(2,0) * mat2(2,0);
    }

    void rewriteAsZero(Mat4x4 &mat){
        mat(0,0) = 0;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
        mat(1,0) = 0;    mat(1,1) = 0;    mat(1,2) = 0;    mat(1,3) = 0;
        mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 0;    mat(2,3) = 0;
        mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 0;
    }

    void rewriteAsIdentity(Mat4x4 &mat){
        mat(0,0) = 1;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
        mat(1,0) = 0;    mat(1,1) = 1;    mat(1,2) = 0;    mat(1,3) = 0;
        mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 1;    mat(2,3) = 0;
        mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
    }

    void rewriteAsScale(Mat4x4 &mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z){
        mat(0,0) = X;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
        mat(1,0) = 0;    mat(1,1) = Y;    mat(1,2) = 0;    mat(1,3) = 0;
        mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = Z;    mat(2,3) = 0;
        mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
    }

    void rewriteAsUniformScale(Mat4x4 &mat, DUA_matrixVal U){
        mat(0,0) = U;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
        mat(1,0) = 0;    mat(1,1) = U;    mat(1,2) = 0;    mat(1,3) = 0;
        mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = U;    mat(2,3) = 0;
        mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
    }

    void rewriteAsTranslation(Mat4x4 &mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z){
        mat(0,0) = 1;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = X;
        mat(1,0) = 0;    mat(1,1) = 1;    mat(1,2) = 0;    mat(1,3) = Y;
        mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 1;    mat(2,3) = Z;
        mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
    }

    void rewriteAsRotationWorldAxisZyx(Mat4x4 &mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z){
        DUA_matrixVal cx = cos(X);
        DUA_matrixVal sx = sin(X);
        DUA_matrixVal cy = cos(Y);
        DUA_matrixVal sy = sin(Y);
        DUA_matrixVal cz = cos(Z);
        DUA_matrixVal sz = sin(Z);

        //row 0
        mat(0,0) =    cy * cx               ;    mat(0,1) =    -cy * sz             ;    mat(0,2) =    sy                   ;    mat(0,3) =                    0    ;
        //row 1
        mat(1,0) =    cx * sz + (sx*sy*cz)  ;    mat(1,1) =    cx * cz - (sx*sy*sz) ;    mat(1,2) =    -sx * cy             ;    mat(1,3) =                    0    ;
        //row 2
        mat(2,0) =    sx * sz - (cx*sy*cz)  ;    mat(2,1) =    sx * cz + (cx*sy*sz) ;    mat(2,2) =    cx * cy              ;    mat(2,3) =                    0    ;
        //row 3
        mat(3,0) =                    0     ;    mat(3,1) =                    0    ;    mat(3,2) =                    0    ;    mat(3,3) =                    1    ;
    }

    void rewriteAsRotationWorldAxisZyx(Mat4x4 &mat, Vec3 &rotation){
        rewriteAsRotationWorldAxisZyx(mat, rotation(0, 0), rotation(1, 0), rotation(2, 0));
    }

    void rewriteAsRotationArbitraryAxis(Mat4x4 &mat, Vec4 &unitAxis, DUA_matrixVal angle){
        DUA_matrixVal c = cos(angle);
        DUA_matrixVal s = sin(angle);
        DUA_matrixVal t = 1.0 - cos(angle);
        DUA_matrixVal x = unitAxis(0,0);
        DUA_matrixVal y = unitAxis(1,0);
        DUA_matrixVal z = unitAxis(2,0);
        DUA_matrixVal x2 = pow(x, 2);
        DUA_matrixVal y2 = pow(y, 2);
        DUA_matrixVal z2 = pow(z, 2);

        //row 0
        mat(0,0) =    t * x2 + c        ;    mat(0,1) =    t * x * y - s * z ;    mat(0,2) =    t * x *z + s * y  ;    mat(0,3) =                    0 ;
        //row 1
        mat(1,0) =    t * x * y + s * z ;    mat(1,1) =    t * y2  + c       ;    mat(1,2) =    t * y * z - s * x ;    mat(1,3) =                    0 ;
        //row 2
        mat(2,0) =    t * x * z - s * y ;    mat(2,1) =    t * y * z + s * x ;    mat(2,2) =    t * z2  + c       ;    mat(2,3) =                    0 ;
        //row 3
        mat(3,0) =                    0 ;    mat(3,1) =                    0 ;    mat(3,2) =                    0 ;    mat(3,3) =                    1 ;
    }

    void rewriteAsViewFree(Mat4x4 &mat, Vec4 &center, Vec4 *axes){
        //row 0
        mat(0,0) =    axes[0](0,0)      ;    mat(0,1) =    axes[0](1,0)     ;    mat(0,2) =    axes[0](2,0)     ;    mat(0,3) =  -dotVec4(axes[0], center) ;
        //row 1
        mat(1,0) =    axes[1](0,0)      ;    mat(1,1) =    axes[1](1,0)     ;    mat(1,2) =    axes[1](2,0)     ;    mat(1,3) =  -dotVec4(axes[1], center) ;
        //row 2
        mat(2,0) =    axes[2](0,0)      ;    mat(2,1) =    axes[2](1,0)     ;    mat(2,2) =    axes[2](2,0)     ;    mat(2,3) =  -dotVec4(axes[2], center) ;
        //row 3
        mat(3,0) =                    0 ;    mat(3,1) =                   0 ;    mat(3,2) =                   0 ;    mat(3,3) =                           1 ;
    }

    void rewriteAsViewFps(Mat4x4 &mat, Vec4 &center, DUA_matrixVal pitch, DUA_matrixVal yaw){
        DUA_matrixVal cosPitch = cos(pitch);
        DUA_matrixVal sinPitch = sin(pitch);
        DUA_matrixVal cosYaw = cos(yaw);
        DUA_matrixVal sinYaw = sin(yaw);

        Vec4 axes[] = {    // Three Vec4d objects representing the camera axes
            {{cosYaw},{0},{-sinYaw},{1}},
            {{sinYaw*sinPitch},{cosPitch},{cosYaw*sinPitch},{1}},
            {{sinYaw*cosPitch},{-sinPitch},{cosPitch * cosYaw},{1}}
        };

        rewriteAsViewFree(mat, center, axes);
    }

    void rewriteAsProjection(Mat4x4 &mat, DUA_matrixVal FOV, DUA_matrixVal aspRat, DUA_matrixVal zNear,
                             DUA_matrixVal zFar){
        DUA_matrixVal tanFOV = tan(FOV);
        DUA_matrixVal zDiff = zFar - zNear;

        //row 0
        mat(0,0) =    1 / tanFOV        ;    mat(0,1) =             0       ;    mat(0,2) =                 0           ;    mat(0,3) =                 0           ;
        //row 1
        mat(1,0) =              0       ;    mat(1,1) =    aspRat / tanFOV  ;    mat(1,2) =                 0           ;    mat(1,3) =                 0           ;
        //row 2
        mat(2,0) =              0       ;    mat(2,1) =             0       ;    mat(2,2) =    (zFar + zNear) / zDiff   ;    mat(2,3) =  -2 * zFar * zNear / zDiff  ;
        //row 3
        mat(3,0) =              0       ;    mat(3,1) =             0       ;    mat(3,2) =                 1           ;    mat(3,3) =                 0           ;
    }




    void transformMatrixTranslate(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z){
        mat(0,3) += X;
        mat(1,3) += Y;
        mat(2,3) += Z;
    }

    void transformMatrixScale(DualityEngine::Mat4x4& mat, DUA_matrixVal X, DUA_matrixVal Y, DUA_matrixVal Z){
        mat(0,0) *= X;
        mat(1,1) *= Y;
        mat(2,2) *= Z;
    }

}