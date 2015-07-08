#include "../Headers/Control_SS.h"

using namespace DualityEngine;

System_Control_SS::System_Control_SS(ComponentBank* bank)
                         : System(bank, "Physics Collision System", 2)
{
    requiredComponents.at(0) = CONTROLSS | LINVELOC ;
    requiredComponents.at(1) = CONTROLSS | ORIENTATION;
}

System_Control_SS::~System_Control_SS()
{
    
}

bool System_Control_SS::init(std::stringstream& output)
{
    return true;
}

void System_Control_SS::tick()
{
    
    for (auto ID : registeredIDs[1]){
        if (bank->getState(ID) & (ACTIVE | RECALCVIEWMAT) == (ACTIVE | RECALCVIEWMAT)){
            bank->getSpaceControlPtr(ID)->transform(bank->getRotMat(ID));
        }
    }
    
    for (auto ID : registeredIDs[0]){
        
    }
    
}
