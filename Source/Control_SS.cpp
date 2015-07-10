#include "../Headers/Control_SS.h"
#include "Maths.h"
#include "ControlBase.h"

using namespace DualityEngine;

System_Control_SS::System_Control_SS(ComponentBank* bank)
                         : System(bank, "Space Ship Control System", 0)
{
    
}

System_Control_SS::~System_Control_SS()
{
    
}

bool System_Control_SS::init(std::stringstream& output)
{
    return true;
}

void System_Control_SS::zeroSum(){
    for (uint i = 0; i < 3; ++i) {
        reusableVectorSum[i] = 0;
    }
}

void System_Control_SS::tick()
{
    // if control focus has been assigned to an existing entity...
    if (bank->activeControlID != DUA_NULL_ID && bank->currentControlType == ControlTypes::SPACE){
        
        
        // store its state since I reference it more than once...
        DUA_stateFlag tempFlag = bank->getState(bank->activeControlID);
        
        // if entity is in active state...
        if (tempFlag & ACTIVE){  
            // if the entity has moved since last time...            
            if (tempFlag & RECALCVIEWMAT){
                // align the control component's axes with its entity's orientation
                bank->pSpaceControlCurrent->transform(bank->getRotMat(bank->activeControlID));
            }
            
            // zero the vector that will hold the total physics impulse for the control object
            zeroSum();
            
            // add all the vectors being applied currently by the user's input into the impulse vector
            for (uint i = 0; i < 6; ++i){
                reusableVectorSum += bank->pSpaceControlCurrent->currentAxes[i / 2] * (bank->pSpaceControlCurrent->inputs[i] * bank->pSpaceControlCurrent->thrust[i] * ((i % 2) ? -1 : 1));
            }
            
            /* APPLY BRAKING */
            
            // apply the impusle vector to the entity's linear velocity component (possesion of a linear velocity component is a requirement for an entity to become the control focus)
            bank->getLinearVelocPtr(bank->activeControlID)->applyImpulse(reusableVectorSum);
            
            // zero the vector sum again to reuse it as angular velocity impulse
            zeroSum();
            for (uint i = 0; i < 3; ++i){
                /* SUM ANG VELOC IMPULSES */
            }
            /* APPLY ANGULAR BRAKING */
            /* APPLY SUM TO ANG VELOC */
            
            // zero the control component's input information after use.
            bank->pSpaceControlCurrent->zeroInputs();
        }
    }
    
}
