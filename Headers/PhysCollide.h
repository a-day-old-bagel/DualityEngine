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

class System_PhysCollide : public System
{
public:
    System_PhysCollide(ComponentBank* bank);
    ~System_PhysCollide();
    void tick() override;
    bool init() override;
};

#endif	/* PHYSCOLLIDE_H */

