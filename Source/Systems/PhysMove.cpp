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

void System_PhysMove::tickImpl()
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
    
    SDL_Delay(5);
}

void System_PhysMove::tockImpl() {
#ifdef DUA_DEBUG_THREADS
	bank->dlgt->output("Physics Thread Running...\n");
#endif
}
