#include "../Headers/PhysCollide.h"

using namespace DualityEngine;

System_PhysCollide::System_PhysCollide(ComponentBank* bank)
                         : System(bank, "Physics Collision System", 1)
{
    
}

System_PhysCollide::~System_PhysCollide()
{
    
}

bool System_PhysCollide::init(std::stringstream& output)
{
    requiredComponents.at(0) = COLLISION | POSITION ;
    return true;
}

void System_PhysCollide::tick()
{
    
}
