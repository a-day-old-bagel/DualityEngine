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
#include "extendGlm.h"

namespace DualityEngine {

    struct Position : public Component
    {
        Position(const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ){
            position = {posX, posY, posZ};
            lastPosition = position;
            timeStamp = 0;
            lastTimeStamp = 0;
//            matrixIsCurrent = false;
        }
//        inline void updateMatrix(){
//            transform = glm::translate(Constants::duaIdentMat4, position);
//            matrixIsCurrent = true;
//        }
        inline glm::mat4 getMatrix(DUA_uint32 time){
            glm::vec3 interp;
            vec3Interpolate(lastPosition, position, interp,
                            (float) (time - lastTimeStamp) /
                            (float) (timeStamp - lastTimeStamp));

            return {{1.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f},
                    {0.f, 0.f, 1.f, 0.f}, {interp.x, interp.y, interp.z, 1.f}};
        }
        inline glm::mat4 getMatrix() {
//            if (!matrixIsCurrent){
//                updateMatrix();
//            }
//            return transform;
            return {{1.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f},
                    {0.f, 0.f, 1.f, 0.f}, {position.x, position.y, position.z, 1.f}};
        }
        inline void translate(const DUA_dbl &x, const DUA_dbl &y, const DUA_dbl &z, DUA_uint32 time){
            lastTimeStamp = timeStamp;
            timeStamp = time;
            lastPosition = position;
            position.x += x;
            position.y += y;
            position.z += z;
//            matrixIsCurrent = false;
        }
        inline void translate(const glm::vec3 &translation, DUA_uint32 timeStamp){
            translate(translation.x, translation.y, translation.z, timeStamp);
        }
        inline void translate(const glm::vec4 &translation, DUA_uint32 timeStamp){
            translate(translation.x, translation.y, translation.z, timeStamp);
        }

        DUA_uint32 timeStamp, lastTimeStamp;
        glm::vec3 position, lastPosition;
//        glm::mat4 transform;    // TOTAL sum including parents
//        bool matrixIsCurrent;
    };

}

#endif	/* POSITION_H */

