#include "PhysMove.h"

using namespace DualityEngine;

System_PhysMove::System_PhysMove(ComponentBank* bank)
                         : System<System_PhysMove>(bank, "Physics Movement System", 2)
{
    requiredComponents.at(0) = POSITION | LINVELOC ;
    requiredComponents.at(1) = ORIENTATION | ANGVELOC ;
	System::tockFreq = 1000;
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
	for (unsigned i = 0; i < registeredIDs[0].size(); ++i) {
        if (bank->getState(registeredIDs[0][i]) & ACTIVE){
            bank->getPositionPtr(registeredIDs[0][i])->translate(bank->getLinearVelocPtr(registeredIDs[0][i])->velLinear);
            bank->stateOn(registeredIDs[0][i], RECALCVIEWMAT);
        }
    }
	for (unsigned i = 0; i < registeredIDs[1].size(); ++i) {
        if (bank->getState(registeredIDs[1][i]) & ACTIVE){
            bank->getOrientationPtr(registeredIDs[1][i])->rotate(bank->getAngularVelocPtr(registeredIDs[1][i])->velAngular);
            bank->stateOn(registeredIDs[1][i], RECALCVIEWMAT);
        }
    }
    
    SDL_Delay(5);
}

