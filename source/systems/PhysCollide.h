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
        System_PhysCollide(Bank * bank);
        void onTick();
        bool onInit(std::stringstream& output);
        void onClean();
    };

}

#endif	/* PHYSCOLLIDE_H */

