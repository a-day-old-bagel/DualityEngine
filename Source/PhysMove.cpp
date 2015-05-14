#include "../Headers/PhysMove.h"

using namespace DualityEngine;

System_PhysMove::System_PhysMove(ComponentBank* bank)
                         : System(bank, "Physics Movement System", 1)
{
    
}

System_PhysMove::~System_PhysMove()
{
    
}

bool System_PhysMove::init(std::stringstream& output)
{
    requiredComponents.at(0) = POSITION | LINVELOC ;
    return true;
}

void System_PhysMove::tick()
{
    
}