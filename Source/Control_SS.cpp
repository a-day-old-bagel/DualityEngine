#include "../Headers/Control_SS.h"
#include "Maths.h"

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

void System_Control_SS::tick()
{
    
    if (bank->activeSpaceControlID != DUA_NULL_ID){
        DUA_stateFlag tempFlag = bank->getState(bank->activeSpaceControlID);
        
        if (tempFlag & ACTIVE){  
            
            if (tempFlag & RECALCVIEWMAT){
                bank->pSpaceControlCurrent->transform(bank->getRotMat(bank->activeSpaceControlID));
            }

            reusableVectorSum = {0, 0, 0};            
            for (uint i = 0; i < 6; ++i){
                reusableVectorSum += bank->pSpaceControlCurrent->currentAxes[i / 2] * (bank->pSpaceControlCurrent->inputs[i] * bank->pSpaceControlCurrent->thrust[i] * -(i % 2));
            }
            bank->getLinearVelocPtr(bank->activeSpaceControlID)->applyImpulse(reusableVectorSum);
            if (reusableVectorSum != glm::vec3(0)){
                bank->dlgt->outputStr(std::to_string(reusableVectorSum.x) + " " + std::to_string(reusableVectorSum.y) + " " + std::to_string(reusableVectorSum.z));
            }
            bank->pSpaceControlCurrent->zeroInputs();
        }
    }
    
}
