/* 
 * File:   Control.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 12:09 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

#include <glm/glm.hpp>
#include "Component.h"

namespace DualityEngine {

    struct Control : public Component
    {
        Control(){
            
        }
        inline void transform(glm::mat4 transform){
            forward = transform * forwardOrig;
            right = transform * rightOrig;
            up = transform * upOrig;
        }
        glm::vec4 forwardOrig = {0, 0, -1, 1};
        glm::vec4 rightOrig = {1, 0, 0, 1};
        glm::vec4 upOrig = {0, 1, 0, 1};
        
        glm::vec4 forward = forwardOrig;
        glm::vec4 right = rightOrig;
        glm::vec4 up = upOrig;
    };

}

#endif	/* CONTROL_H */

