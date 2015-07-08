#include "../Headers/PhysMove.h"

using namespace DualityEngine;

System_PhysMove::System_PhysMove(ComponentBank* bank)
                         : System(bank, "Physics Movement System", 2)
{
    requiredComponents.at(0) = POSITION | LINVELOC ;
    requiredComponents.at(1) = ORIENTATION | ANGVELOC ;
    
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
//    for (auto ID : registeredIDs[0]){
//        if (bank->getState(ID) & ACTIVE){
//            positionPtr = bank->getPositionPtr(ID);
//            if (positionPtr != NULL){
//                linVelocPtr = bank->getLinearVelocPtr(ID);
//                if (linVelocPtr != NULL){
//                    positionPtr->translate(linVelocPtr->velLinear);
//                    bank->stateOn(ID, RECALCVIEWMAT);
//                }
//            }
//        }
//    }
//    for (auto ID : registeredIDs[1]){
//        if (bank->getState(ID) & ACTIVE){
//            orientationPtr = bank->getOrientationPtr(ID);
//            if (orientationPtr != NULL){
//                angVelocPtr = bank->getAngularVelocPtr(ID);
//                if (angVelocPtr != NULL){
//                    orientationPtr->rotate(angVelocPtr->velAngular);
//                    bank->stateOn(ID, RECALCVIEWMAT);
//                }
//            }
//        }
//    }
    
    
    for (auto ID : registeredIDs[0]){
        if (bank->getState(ID) & ACTIVE){
            bank->getPositionPtr(ID)->translate(bank->getLinearVelocPtr(ID)->velLinear);
            bank->stateOn(ID, RECALCVIEWMAT);
        }
    }
    bool hasDoneOne = true;
    if (registeredIDs[1].size())
        bool hasDoneOne = false;
    for (auto ID : registeredIDs[1]){
        if (bank->getState(ID) & ACTIVE){
            hasDoneOne = true;
            bank->getOrientationPtr(ID)->rotate(bank->getAngularVelocPtr(ID)->velAngular);
            bank->stateOn(ID, RECALCVIEWMAT); 
            
        }
    }
    if (!hasDoneOne) std::cout << "rotateFail\n";
    
    SDL_Delay(2);
}