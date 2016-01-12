/* 
 * File:   PhysMove.h
 * Author: adayoldbagel
 *
 * Created on February 11, 2015, 11:26 PM
 */

#ifndef PHYSMOVE_H
#define	PHYSMOVE_H

#include "System.h"
#include <glm/glm.hpp>
#include <SDL.h>

namespace DualityEngine {

    class System_PhysMove : public System<System_PhysMove>
    {
//        uint ticker = 0;
        
//        Position* positionPtr;
//        LinearVelocity* linVelocPtr;
//        Orientation* orientationPtr;
//        AngularVelocity* angVelocPtr;
        
    public:
        System_PhysMove(ComponentBank* bank);
        ~System_PhysMove();
        void tickImpl();
		void tockImpl();
        bool init(std::stringstream& output);
    };

}

#endif	/* PHYSMOVE_H */

