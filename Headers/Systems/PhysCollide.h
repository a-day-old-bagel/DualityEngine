/* 
 * File:   PhysCollide.h
 * Author: adayoldbagel
 *
 * Created on February 11, 2015, 11:26 PM
 */

#ifndef PHYSCOLLIDE_H
#define	PHYSCOLLIDE_H

#include "System.h"
#include <glm/glm.hpp>

namespace DualityEngine {

    class System_PhysCollide : public System<System_PhysCollide>
    {
    public:
        System_PhysCollide(ComponentBank* bank);
        ~System_PhysCollide();
        void tick();
        bool init(std::stringstream& output);
    };

}

#endif	/* PHYSCOLLIDE_H */

