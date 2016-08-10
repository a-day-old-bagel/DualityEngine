/* 
 * File:   CameraFree.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef CAMERA_FREE_H
#define	CAMERA_FREE_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>
#include "settings.h"
#include "Component.h"

namespace DualityEngine {

    struct CameraFree : public Component
    {
        CameraFree(DUA_float fov, DUA_float zNear, DUA_float zFar, DUA_dbl eyeX, DUA_dbl eyeY, DUA_dbl eyeZ, DUA_dbl focusX, DUA_dbl focusY, DUA_dbl focusZ, DUA_dbl upX, DUA_dbl upY, DUA_dbl upZ){
            this->fov = fov;
            this->zNear = zNear;
            this->zFar = zFar;
            eyeOrig = {eyeX, eyeY, eyeZ, 1.0};
            focusOrig = {focusX, focusY, focusZ, 1.0};
            upOrig = {upX, upY, upZ, 1.0};
            updateView(glm::mat4(1.0), glm::mat4(1.0));
            updateProjection();
            updateViewProjection();
        }
        inline void updateView(glm::mat4 rotation, glm::mat4 translation){
            glm::mat4 transform = translation * rotation;
            view = glm::lookAt(glm::vec3(transform * eyeOrig), glm::vec3(transform * focusOrig), glm::vec3(rotation * upOrig));            
        }
        inline void updateProjection(){
            projection = glm::perspective(fov, aspect, zNear, zFar);
        }
        inline void updateViewProjection(){
            viewProjection = projection * view;
        }

        DUA_float fov;
        DUA_float aspect = Settings::Display::screenAspectRatio;
        DUA_float zNear;
        DUA_float zFar;

        glm::vec4 eyeOrig;
        glm::vec4 focusOrig;
        glm::vec4 upOrig;
        
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 viewProjection;
    };

}

#endif	/* CAMERA_FREE_H */

