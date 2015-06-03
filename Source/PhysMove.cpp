#include "../Headers/PhysMove.h"

using namespace DualityEngine;

System_PhysMove::System_PhysMove(ComponentBank* bank)
                         : System(bank, "Physics Movement System", 2)
{
    requiredComponents.at(0) = POSITION | LINVELOC ;
    requiredComponents.at(1) = ROTATION | ANGVELOC ;
}

System_PhysMove::~System_PhysMove()
{
    
}

bool System_PhysMove::init(std::stringstream& output)
{
    return true;
}

void System_PhysMove::tick()
{
    for (auto ID : registeredIDs[0]){
        if (!(bank->getState(ID) & INACTIVE)){
            bank->getPositionPtr(ID)->position += bank->getPositionVelocPtr(ID)->velLinear;
        }
    }
    for (auto ID : registeredIDs[1]){
        if (!(bank->getState(ID) & INACTIVE)){
            bank->getRotationPtr(ID)->rotation += bank->getRotationVelocPtr(ID)->velAngular;
        }
    }
}