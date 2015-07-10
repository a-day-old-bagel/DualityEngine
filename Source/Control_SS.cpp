#include "../Headers/Control_SS.h"
#include <algorithm>
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
            
            
            // if linear breaking input is non-zero
            if (bank->pSpaceControlCurrent->throttle[9]){ // apply braking vector to throttle values...                
                // store the entity's current velocity
                reusableVectorVel = bank->getLinearVelocPtr(bank->activeControlID)->velLinear;
                // calculate dot products between velocity vector and thrust vectors
                for (uint i = 0; i < 3; ++i){
                    reusableDotProductsForBraking[i] = glm::dot(reusableVectorVel, bank->pSpaceControlCurrent->currentAxes[i]);
                }                
                // find which dot product largest in magnitude in order to scale that thruster's throttle to full (scaling others by same factor, resulting in less-than-full throttles)
                if (fabs(reusableDotProductsForBraking[0]) >= fabs(reusableDotProductsForBraking[1])){
                    if (fabs(reusableDotProductsForBraking[0]) >= fabs(reusableDotProductsForBraking[2])){
                        breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[0]);
                        bank->dlgt->outputStr("mult: " + std::to_string(breakingVectorComponentMultiplier));
                    }else{
                        breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[2]);
                    }                    
                }else if (fabs(reusableDotProductsForBraking[1]) >= fabs(reusableDotProductsForBraking[2])){
                    breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[1]);
                }else{
                    breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[2]);
                }
                // add the appropriate values to the control component's throttle array (allowing no value to pass 1)
                for (uint i = 0; i < 3; ++i){
                    bank->pSpaceControlCurrent->throttle[i * 2 + int(reusableDotProductsForBraking[i] > 0)] += fabs(reusableDotProductsForBraking[i]) * breakingVectorComponentMultiplier * bank->pSpaceControlCurrent->throttle[9];
                    bank->dlgt->outputStr(std::to_string(fabs(reusableDotProductsForBraking[i]) * breakingVectorComponentMultiplier * bank->pSpaceControlCurrent->throttle[9]) + " ");
                    bank->dlgt->outputStr("thing " + std::to_string(i * 2 + int(reusableDotProductsForBraking[i] < 0)));
                }
                bank->dlgt->outputStr(std::to_string(bank->pSpaceControlCurrent->throttle[0]) + " "
                                    + std::to_string(bank->pSpaceControlCurrent->throttle[1]) + " "
                                    + std::to_string(bank->pSpaceControlCurrent->throttle[2]) + " "
                                    + std::to_string(bank->pSpaceControlCurrent->throttle[3]) + " "
                                    + std::to_string(bank->pSpaceControlCurrent->throttle[4]) + " "
                                    + std::to_string(bank->pSpaceControlCurrent->throttle[5]));
            }
            
            
            // zero the vector that will hold the total physics impulse for the control object
            zeroSum();
            
//            // add all the vectors being applied currently by the user's input into the impulse vector
//            for (uint i = 0; i < 6; ++i){
//                reusableVectorSum += bank->pSpaceControlCurrent->currentAxes[i / 2] * (std::min(bank->pSpaceControlCurrent->throttle[i], 1.0) * bank->pSpaceControlCurrent->thrust[i] * ((i % 2) ? -1 : 1));
//            }
            
            for (uint i = 0; i < 3; ++i){
                reusableVectorSum += bank->pSpaceControlCurrent->currentAxes[i] * (std::min(bank->pSpaceControlCurrent->throttle[i * 2], 1.0f) * bank->pSpaceControlCurrent->thrust[i * 2]) -
                                     bank->pSpaceControlCurrent->currentAxes[i] * (std::min(bank->pSpaceControlCurrent->throttle[i * 2 + 1], 1.0f) * bank->pSpaceControlCurrent->thrust[i * 2 + 1]);
            }
            
            // apply the total impusle vector to the entity's linear velocity component (possesion of a linear velocity component is a requirement for an entity to become the control focus)
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
