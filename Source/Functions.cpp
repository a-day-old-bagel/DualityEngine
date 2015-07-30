#include "../Headers/Functions.h"
#include <cmath>

using namespace DualityEngine;

glm::vec3 operator * (glm::vec3 vec, float scalar){
    return glm::vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

double dotVec4D(DualityEngine::Vec4d& mat1, DualityEngine::Vec4d& mat2){
    return mat1(0,0) * mat2(0,0) + mat1(1,0) * mat2(1,0) + mat1(2,0) * mat2(2,0) + mat1(3,0) * mat2(3,0);
}

float dotVec4F(DualityEngine::Vec4f& mat1, DualityEngine::Vec4f& mat2){
    return mat1(0,0) * mat2(0,0) + mat1(1,0) * mat2(1,0) + mat1(2,0) * mat2(2,0) + mat1(3,0) * mat2(3,0);
}

double dotVec3D(DualityEngine::Vec3d& mat1, DualityEngine::Vec3d& mat2){
    return mat1(0,0) * mat2(0,0) + mat1(1,0) * mat2(1,0) + mat1(2,0) * mat2(2,0);
}

float dotVec3F(DualityEngine::Vec3f& mat1, DualityEngine::Vec3f& mat2){
    return mat1(0,0) * mat2(0,0) + mat1(1,0) * mat2(1,0) + mat1(2,0) * mat2(2,0);
}

void convertToZeroMatrixD(Mat4x4d& mat){
    mat(0,0) = 0;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = 0;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 0;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 0;
}

void convertToZeroMatriF(Mat4x4f& mat){
    mat(0,0) = 0;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = 0;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 0;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 0;
}

void convertToIdentityMatrixD(Mat4x4d& mat){
    mat(0,0) = 1;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = 1;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 1;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToIdentityMatrixF(Mat4x4f& mat){
    mat(0,0) = 1;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = 1;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 1;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToScaleMatrixD(Mat4x4d& mat, double X, double Y, double Z){
    mat(0,0) = X;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = Y;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = Z;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToScaleMatrixF(Mat4x4f& mat, float X, float Y, float Z){
    mat(0,0) = X;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = Y;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = Z;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToUniformScaleMatrixD(Mat4x4d& mat, double U){
    mat(0,0) = U;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = U;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = U;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToUniformScaleMatrixF(Mat4x4f& mat, float U){
    mat(0,0) = U;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = 0;
    mat(1,0) = 0;    mat(1,1) = U;    mat(1,2) = 0;    mat(1,3) = 0;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = U;    mat(2,3) = 0;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToTranslationMatrixD(Mat4x4d& mat, double X, double Y, double Z){
    mat(0,0) = 1;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = X;
    mat(1,0) = 0;    mat(1,1) = 1;    mat(1,2) = 0;    mat(1,3) = Y;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 1;    mat(2,3) = Z;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToTranslationMatrixF(Mat4x4f& mat, float X, float Y, float Z){
    mat(0,0) = 1;    mat(0,1) = 0;    mat(0,2) = 0;    mat(0,3) = X;
    mat(1,0) = 0;    mat(1,1) = 1;    mat(1,2) = 0;    mat(1,3) = Y;
    mat(2,0) = 0;    mat(2,1) = 0;    mat(2,2) = 1;    mat(2,3) = Z;
    mat(3,0) = 0;    mat(3,1) = 0;    mat(3,2) = 0;    mat(3,3) = 1;
}

void convertToRotationMatrixWorldAxisZyxD(Mat4x4d& mat, double X, double Y, double Z){
    double cx = cos(X);
    double sx = sin(X);
    double cy = cos(Y);
    double sy = sin(Y);
    double cz = cos(Z);
    double sz = sin(Z);
    
    //row 0
    mat(0,0) =    cy * cx               ;    mat(0,1) =    -cy * sz             ;    mat(0,2) =    sy                   ;    mat(0,3) =                    0    ;
    //row 1
    mat(1,0) =    cx * sz + (sx*sy*cz)  ;    mat(1,1) =    cx * cz - (sx*sy*sz) ;    mat(1,2) =    -sx * cy             ;    mat(1,3) =                    0    ;
    //row 2
    mat(2,0) =    sx * sz - (cx*sy*cz)  ;    mat(2,1) =    sx * cz + (cx*sy*sz) ;    mat(2,2) =    cx * cy              ;    mat(2,3) =                    0    ;
    //row 3
    mat(3,0) =                    0     ;    mat(3,1) =                    0    ;    mat(3,2) =                    0    ;    mat(3,3) =                    1    ;
}

void convertToRotationMatrixWorldAxisZyxF(Mat4x4f& mat, float X, float Y, float Z){
    float cx = cos(X);
    float sx = sin(X);
    float cy = cos(Y);
    float sy = sin(Y);
    float cz = cos(Z);
    float sz = sin(Z);
    
    //row 0
    mat(0,0) =    cy * cx               ;    mat(0,1) =    -cy * sz             ;    mat(0,2) =    sy                   ;    mat(0,3) =                    0    ;
    //row 1
    mat(1,0) =    cx * sz + (sx*sy*cz)  ;    mat(1,1) =    cx * cz - (sx*sy*sz) ;    mat(1,2) =    -sx * cy             ;    mat(1,3) =                    0    ;
    //row 2
    mat(2,0) =    sx * sz - (cx*sy*cz)  ;    mat(2,1) =    sx * cz + (cx*sy*sz) ;    mat(2,2) =    cx * cy              ;    mat(2,3) =                    0    ;
    //row 3
    mat(3,0) =                    0     ;    mat(3,1) =                    0    ;    mat(3,2) =                    0    ;    mat(3,3) =                    1    ;
}

void convertToRotationMatrixWorldAxisZyxD(Mat4x4d& mat, Vec3d& rotation){
    convertToRotationMatrixWorldAxisZyxD(mat, rotation(0,0), rotation(1,0), rotation(2,0));
}

void convertToRotationMatrixWorldAxisZyxF(Mat4x4f& mat, Vec3f& rotation){
    convertToRotationMatrixWorldAxisZyxF(mat, rotation(0,0), rotation(1,0), rotation(2,0));
}

void convertToRotationMatrixArbitraryAxisD(Mat4x4d& mat, Vec4d& unitAxis, double angle){
    double c = cos(angle);
    double s = sin(angle);
    double t = 1.0 - cos(angle);
    double x = unitAxis(0,0);
    double y = unitAxis(1,0);
    double z = unitAxis(2,0);
    double x2 = pow(x, 2);
    double y2 = pow(y, 2);
    double z2 = pow(z, 2);
    
    //row 0
    mat(0,0) =    t * x2 + c        ;    mat(0,1) =    t * x * y - s * z ;    mat(0,2) =    t * x *z + s * y  ;    mat(0,3) =                    0 ;
    //row 1
    mat(1,0) =    t * x * y + s * z ;    mat(1,1) =    t * y2  + c       ;    mat(1,2) =    t * y * z - s * x ;    mat(1,3) =                    0 ;
    //row 2
    mat(2,0) =    t * x * z - s * y ;    mat(2,1) =    t * y * z + s * x ;    mat(2,2) =    t * z2  + c       ;    mat(2,3) =                    0 ;
    //row 3
    mat(3,0) =                    0 ;    mat(3,1) =                    0 ;    mat(3,2) =                    0 ;    mat(3,3) =                    1 ;
}

void convertToRotationMatrixArbitraryAxisF(Mat4x4f& mat, Vec4f& unitAxis, float angle){
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0 - cos(angle);
    float x = unitAxis(0,0);
    float y = unitAxis(1,0);
    float z = unitAxis(2,0);
    float x2 = pow(x, 2);
    float y2 = pow(y, 2);
    float z2 = pow(z, 2);
    
    //row 0
    mat(0,0) =    t * x2 + c        ;    mat(0,1) =    t * x * y - s * z ;    mat(0,2) =    t * x *z + s * y  ;    mat(0,3) =                    0 ;
    //row 1
    mat(1,0) =    t * x * y + s * z ;    mat(1,1) =    t * y2  + c       ;    mat(1,2) =    t * y * z - s * x ;    mat(1,3) =                    0 ;
    //row 2
    mat(2,0) =    t * x * z - s * y ;    mat(2,1) =    t * y * z + s * x ;    mat(2,2) =    t * z2  + c       ;    mat(2,3) =                    0 ;
    //row 3
    mat(3,0) =                    0 ;    mat(3,1) =                    0 ;    mat(3,2) =                    0 ;    mat(3,3) =                    1 ;
}

void convertToViewMatrixFreeD(Mat4x4d& mat, Vec4d& center, Vec4d* axes){
    //row 0
    mat(0,0) =    axes[0](0,0)      ;    mat(0,1) =    axes[0](1,0)     ;    mat(0,2) =    axes[0](2,0)     ;    mat(0,3) =  -dotVec4D(axes[0], center) ;
    //row 1
    mat(1,0) =    axes[1](0,0)      ;    mat(1,1) =    axes[1](1,0)     ;    mat(1,2) =    axes[1](2,0)     ;    mat(1,3) =  -dotVec4D(axes[1], center) ;
    //row 2
    mat(2,0) =    axes[2](0,0)      ;    mat(2,1) =    axes[2](1,0)     ;    mat(2,2) =    axes[2](2,0)     ;    mat(2,3) =  -dotVec4D(axes[2], center) ;
    //row 3
    mat(3,0) =                    0 ;    mat(3,1) =                   0 ;    mat(3,2) =                   0 ;    mat(3,3) =                    1 ;
}

void convertToViewMatrixFreeF(Mat4x4f& mat, Vec4f& center, Vec4f* axes){
    //row 0
    mat(0,0) =    axes[0](0,0)      ;    mat(0,1) =    axes[0](1,0)     ;    mat(0,2) =    axes[0](2,0)     ;    mat(0,3) =  -dotVec4F(axes[0], center) ;
    //row 1
    mat(1,0) =    axes[1](0,0)      ;    mat(1,1) =    axes[1](1,0)     ;    mat(1,2) =    axes[1](2,0)     ;    mat(1,3) =  -dotVec4F(axes[1], center) ;
    //row 2
    mat(2,0) =    axes[2](0,0)      ;    mat(2,1) =    axes[2](1,0)     ;    mat(2,2) =    axes[2](2,0)     ;    mat(2,3) =  -dotVec4F(axes[2], center) ;
    //row 3
    mat(3,0) =                    0 ;    mat(3,1) =                   0 ;    mat(3,2) =                   0 ;    mat(3,3) =                    1 ;
}

void convertToViewMatrixFpsD(Mat4x4d& mat, Vec4d& center, double pitch, double yaw){
    double cosPitch = cos(pitch);
    double sinPitch = sin(pitch);
    double cosYaw = cos(yaw);
    double sinYaw = sin(yaw);
    
    Vec4d axes[] = {    // Three Vec4d objects representing the camera axes
        {{cosYaw},{0},{-sinYaw},{1}},
        {{sinYaw*sinPitch},{cosPitch},{cosYaw*sinPitch},{1}},
        {{sinYaw*cosPitch},{-sinPitch},{cosPitch * cosYaw},{1}}
    };
    
    convertToViewMatrixFreeD(mat, center, axes);
}

void convertToViewMatrixFpsF(Mat4x4f& mat, Vec4f& center, float pitch, float yaw){
    float cosPitch = cos(pitch);
    float sinPitch = sin(pitch);
    float cosYaw = cos(yaw);
    float sinYaw = sin(yaw);
    
    Vec4f axes[] = {    // Three Vec4d objects representing the camera axes
        {{cosYaw},{0},{-sinYaw},{1}},
        {{sinYaw*sinPitch},{cosPitch},{cosYaw*sinPitch},{1}},
        {{sinYaw*cosPitch},{-sinPitch},{cosPitch * cosYaw},{1}}
    };
    
    convertToViewMatrixFreeF(mat, center, axes);
}

void convertToProjectionMatrixD(Mat4x4d& mat, double FOV, double aspRat, double zNear, double zFar){
    double tanFOV = tan(FOV);
    double zDiff = zFar - zNear;
    
    //row 0
    mat(0,0) =    1 / tanFOV        ;    mat(0,1) =             0       ;    mat(0,2) =                 0           ;    mat(0,3) =                 0           ;
    //row 1
    mat(1,0) =              0       ;    mat(1,1) =    aspRat / tanFOV  ;    mat(1,2) =                 0           ;    mat(1,3) =                 0           ;
    //row 2
    mat(2,0) =              0       ;    mat(2,1) =             0       ;    mat(2,2) =    (zFar + zNear) / zDiff   ;    mat(2,3) =  -2 * zFar * zNear / zDiff  ;
    //row 3
    mat(3,0) =              0       ;    mat(3,1) =             0       ;    mat(3,2) =                 1           ;    mat(3,3) =                 0           ;
}

void convertToProjectionMatrixF(Mat4x4f& mat, float FOV, float aspRat, float zNear, float zFar){
    float tanFOV = tan(FOV);
    float zDiff = zFar - zNear;
    
    //row 0
    mat(0,0) =    1 / tanFOV        ;    mat(0,1) =             0       ;    mat(0,2) =                 0           ;    mat(0,3) =                 0           ;
    //row 1
    mat(1,0) =              0       ;    mat(1,1) =    aspRat / tanFOV  ;    mat(1,2) =                 0           ;    mat(1,3) =                 0           ;
    //row 2
    mat(2,0) =              0       ;    mat(2,1) =             0       ;    mat(2,2) =    (zFar + zNear) / zDiff   ;    mat(2,3) =  -2 * zFar * zNear / zDiff  ;
    //row 3
    mat(3,0) =              0       ;    mat(3,1) =             0       ;    mat(3,2) =                 1           ;    mat(3,3) =                 0           ;
}