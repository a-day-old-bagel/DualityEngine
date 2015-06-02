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
        CameraFree(DUA_float fov, DUA_float zNear, DUA_float zFar){
            this->fov = fov;
            this->zNear = zNear;
            this->zFar = zFar;
        }
        
        DUA_float fov;
        DUA_float zNear;
        DUA_float zFar;
        
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 viewProjection;
        
        bool viewRecompute = true;
        bool projRecompute = true;
    };

}

#endif	/* CAMERA_FREE_H */

