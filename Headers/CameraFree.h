/* 
 * File:   CameraFree.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef CAMERA_FREE_H
#define	CAMERA_FREE_H

#include <glm/glm.hpp>
#include "Settings.h"
#include "Component.h"

namespace DualityEngine {

    struct CameraFree : public Component
    {
        CameraFree(DUA_float fov, DUA_float zNear, DUA_float zFar, DUA_dbl eyeX, DUA_dbl eyeY, DUA_dbl eyeZ, DUA_dbl focusX, DUA_dbl focusY, DUA_dbl focusZ, DUA_dbl upX, DUA_dbl upY, DUA_dbl upZ){
            this->fov = fov;
            this->zNear = zNear;
            this->zFar = zFar;
            eye = {eyeX, eyeY, eyeZ, 1.0};
            focus = {focusX, focusY, focusZ, 1.0};
            up = {upX, upY, upZ, 1.0};
        }
        
        DUA_float fov;
        DUA_float zNear;
        DUA_float zFar;
        
        glm::vec4 eye;
        glm::vec4 focus;
        glm::vec4 up;
        
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 viewProjection;
        
//        bool viewRecompute = true;
//        bool projRecompute = true;
    };

}

#endif	/* CAMERA_FREE_H */

