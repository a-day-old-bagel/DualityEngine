#include "PhysCollide.h"

using namespace DualityEngine;

System_PhysCollide::System_PhysCollide(ComponentBank* bank)
                         : System(bank, "Physics Collision System", 1)
{
    requiredComponents.at(0) = COLLISION | POSITION ;
}

System_PhysCollide::~System_PhysCollide()
{
    
}

bool System_PhysCollide::init(std::stringstream& output)
{
    return true;
}

void System_PhysCollide::tick()
{
    
}
