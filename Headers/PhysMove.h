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

namespace DualityEngine {

    class System_PhysMove : public System
    {
    public:
        System_PhysMove(ComponentBank* bank);
        ~System_PhysMove();
        void tick() override;
        bool init(std::stringstream& output) override;
    };

}

#endif	/* PHYSMOVE_H */

