/* 
 * File:   Spatial.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef POSITION_H
#define	POSITION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"

namespace DualityEngine {

    struct Position : public Component
    {
        Position(const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ){
            position = {posX, posY, posZ};
            matrixIsCurrent = false;
        }
        inline void updateMatrix(){
            transform = glm::translate(Constants::duaIdentMat4, position);
            matrixIsCurrent = true;
        }
        inline glm::mat4 getMatrix(){
            if (!matrixIsCurrent){
                updateMatrix();
            }
            return transform;
        }
        inline void translate(const DUA_dbl &x, const DUA_dbl &y, const DUA_dbl &z){
            position.x += x;
            position.y += y;
            position.z += z;
            matrixIsCurrent = false;
        }
        inline void translate(const glm::vec3& translation){
            translate(translation.x, translation.y, translation.z);            
        }
        inline void translate(const glm::vec4& translation){
            translate(translation.x, translation.y, translation.z);            
        }
        
        glm::vec3 position;
        glm::mat4 transform;    // TOTAL sum including parents
        bool matrixIsCurrent;
    };

}

#endif	/* POSITION_H */

