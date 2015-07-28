#include "../Headers/Maths.h"
#include <cmath>

using namespace DualityEngine;

glm::vec3 operator * (glm::vec3 vec, float scalar){
    return glm::vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

void makeProjectionMatrixD(Mat4x4d& mat){
    
}

void makeProjectionMatrixF(Mat4x4f& mat){
    
}

void makeRotationMatrixArbitraryAxisD(Mat4x4d& mat, Vec4d& unitAxis, double angle){
    double c = cos(angle);
    double s = sin(angle);
    double t = 1.0 - cos(angle);
    double x = unitAxis(0,0);
    double y = unitAxis(1,0);
    double z = unitAxis(2,0);
    double x2 = pow(x, 2);
    double y2 = pow(y, 2);
    double z2 = pow(z, 2);
}

void makeRotationMatrixArbitraryAxisF(Mat4x4f& mat, Vec4f& unitAxis, float angle){
    
}