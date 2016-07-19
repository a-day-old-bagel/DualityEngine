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
        DUA_uint32 lastTime;
    public:
        System_PhysMove(Bank * bank);
		void onTick();
        bool onInit(std::stringstream& output);
        void onClean();
    };

}

#endif	/* PHYSMOVE_H */

