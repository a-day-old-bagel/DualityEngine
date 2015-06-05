#include "../Headers/PhysMove.h"

using namespace DualityEngine;

System_PhysMove::System_PhysMove(ComponentBank* bank)
                         : System(bank, "Physics Movement System", 3)
{
    requiredComponents.at(0) = POSITION | LINVELOC ;
    requiredComponents.at(1) = ORIENTATION | ANGVELOC ;
    requiredComponents.at(2) = CONTROL | ORIENTATION;
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
        if (bank->getState(ID) & ACTIVE){
            bank->getPositionPtr(ID)->translate(bank->getLinearVelocPtr(ID)->velLinear);
            bank->stateOn(ID, RECALCVIEWMAT);
        }
    }
    for (auto ID : registeredIDs[1]){
        if (bank->getState(ID) & ACTIVE){
            bank->getOrientationPtr(ID)->rotate(bank->getAngularVelocPtr(ID)->velAngular);
            bank->stateOn(ID, RECALCVIEWMAT);            
        }
    }
    for (auto ID : registeredIDs[2]){
        if (bank->getState(ID) & (ACTIVE | RECALCVIEWMAT) == (ACTIVE | RECALCVIEWMAT)){
            bank->getControlPtr(ID)->transform(bank->getRotMat(ID));
        }
    }
    SDL_Delay(10);
}