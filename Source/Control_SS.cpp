#include "../Headers/Control_SS.h"
#include <algorithm>
#include "Functions.h"
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
            if (bank->pSpaceControlCurrent->throttle[ControlSS::LINBRAKE]){ // apply braking vector to throttle values...
                // store the entity's current velocity                
                reusableVectorVel = bank->getLinearVelocPtr(bank->activeControlID)->velLinear;
                if(reusableVectorVel.x || reusableVectorVel.y || reusableVectorVel.z){
                    // calculate dot products between velocity vector and thrust vectors
                    for (uint i = 0; i < 3; ++i){
                        reusableDotProductsForBraking[i] = glm::dot(reusableVectorVel, bank->pSpaceControlCurrent->currentAxes[i]);
//                        if (reusableDotProductsForBraking[i] == 0){
//                            reusableDotProductsForBraking[i] = 0.00000000000000001;
//                            std::cout << "CRISIS AVERTED\n";
//                        }
                    }
                    // find which dot product largest in magnitude in order to scale that thruster's throttle to full (scaling others by same factor, resulting in less-than-full throttles)
                    if (fabs(reusableDotProductsForBraking[0]) >= fabs(reusableDotProductsForBraking[1])){
                        if (fabs(reusableDotProductsForBraking[0]) >= fabs(reusableDotProductsForBraking[2])){
                            breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[0]);
                        }else{
                            breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[2]);
                        }                    
                    }else if (fabs(reusableDotProductsForBraking[1]) >= fabs(reusableDotProductsForBraking[2])){
                        breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[1]);
                    }else{
                        breakingVectorComponentMultiplier = fabs(1.f / reusableDotProductsForBraking[2]);
                    }
                    
                    
                    if (std::isnan(breakingVectorComponentMultiplier))
                    {
                        breakingVectorComponentMultiplier = 0;
                        reusableDotProductsForBraking[0] = 0;
                        reusableDotProductsForBraking[1] = 0;
                        reusableDotProductsForBraking[2] = 0;
                        std::cout << "NAN NAN NAN NAN LINEAR\n";
                        return;
                    }
                    
                    
                    // add the appropriate values to the control component's throttle array (allowing no value to pass 1)
                    for (uint i = 0; i < 3; ++i){
                        // only break in a direction if the user isn't trying to thrust that direction!
                        if (! bank->pSpaceControlCurrent->throttle[i * 2 + int(reusableDotProductsForBraking[i] <= 0)]){
                            // apply braking thrust to the appropriate directions
                            opposingThrustDecider = int(reusableDotProductsForBraking[i] > 0);
                            thrustMostOppositeVelocity = i * 2 + opposingThrustDecider;
                            bank->pSpaceControlCurrent->throttle[thrustMostOppositeVelocity] += fabs(reusableDotProductsForBraking[i]) * breakingVectorComponentMultiplier
                                    * (std::min( (reusableDotProductsForBraking[i] * (-1 + 2 * opposingThrustDecider) ) / bank->pSpaceControlCurrent->thrust[thrustMostOppositeVelocity], 1.f)) //???
                                    * bank->pSpaceControlCurrent->throttle[ControlSS::LINBRAKE];
                        }
                    }
                }
            }

            // if angular breaking input is non-zero
            if (bank->pSpaceControlCurrent->throttle[ControlSS::ANGBRAKE]) { // apply braking vector to throttle values...
                // store the entity's current angular velocity
                reusableVectorVel = bank->getAngularVelocPtr(bank->activeControlID)->velAngular;
                if(reusableVectorVel.x || reusableVectorVel.y || reusableVectorVel.z){
                    // find which angveloc component is largest in order to scale that torques's throttle to full (scaling others by same factor, resulting in less-than-full throttles)
                    if (fabs(reusableVectorVel[0]) >= fabs(reusableVectorVel[1])){
                        if (fabs(reusableVectorVel[0]) >= fabs(reusableVectorVel[2])){
                            breakingVectorComponentMultiplier = fabs(1.f / reusableVectorVel[0]);
                        }else{
                            breakingVectorComponentMultiplier = fabs(1.f / reusableVectorVel[2]);
                        }                    
                    }else if (fabs(reusableVectorVel[1]) >= fabs(reusableVectorVel[2])){
                        breakingVectorComponentMultiplier = fabs(1.f / reusableVectorVel[1]);
                    }else{
                        breakingVectorComponentMultiplier = fabs(1.f / reusableVectorVel[2]);
                    }
                    // add the appropriate values to the control component's throttle array (allowing no value to pass 1)
                    for (uint i = 0; i < 3; ++i){
                        // only break in a direction if the user isn't trying to thrust that direction!
                        if (! bank->pSpaceControlCurrent->throttle[i * 2 + 6 + int(reusableVectorVel[i] <= 0)]){
                            
                            
                            
                            opposingThrustDecider = int(reusableVectorVel[i] > 0);
                            thrustMostOppositeVelocity = i * 2 + 6 + opposingThrustDecider;
                            bank->pSpaceControlCurrent->throttle[thrustMostOppositeVelocity] += fabs(reusableVectorVel[i]) * breakingVectorComponentMultiplier
                                    * (std::min( (reusableVectorVel[i] * (-1 + 2 * opposingThrustDecider) ) / bank->pSpaceControlCurrent->thrust[thrustMostOppositeVelocity], 1.f)) //???
                                    * bank->pSpaceControlCurrent->throttle[ControlSS::ANGBRAKE];
                            
                            
                            
//                            // apply braking thrust to the appropriate directions
//                            bank->pSpaceControlCurrent->throttle[i * 2 + 6 + int(reusableVectorVel[i] > 0)] += fabs(reusableVectorVel[i]) * breakingVectorComponentMultiplier
//                                    * bank->pSpaceControlCurrent->throttle[ControlSS::ANGBRAKE];
                        }
                    }
                }
            }         
            
            bank->getLinearVelocPtr(bank->activeControlID)->applyImpulse(
                bank->pSpaceControlCurrent->currentAxes[0] * (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::FORWARD ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::FORWARD ])
              - bank->pSpaceControlCurrent->currentAxes[0] * (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::BACKWARD], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::BACKWARD])
              + bank->pSpaceControlCurrent->currentAxes[1] * (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::RIGHT   ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::RIGHT   ])
              - bank->pSpaceControlCurrent->currentAxes[1] * (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::LEFT    ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::LEFT    ])
              + bank->pSpaceControlCurrent->currentAxes[2] * (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::UP      ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::UP      ])
              - bank->pSpaceControlCurrent->currentAxes[2] * (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::DOWN    ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::DOWN    ])
            );
            
            bank->getAngularVelocPtr(bank->activeControlID)->applyImpulse(
                (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::PITCHPOS], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::PITCHPOS])
              - (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::PITCHNEG], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::PITCHNEG]),
                (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::YAWPOS  ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::YAWPOS  ])
              - (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::YAWNEG  ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::YAWNEG  ]),
                (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::ROLLPOS ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::ROLLPOS ])
              - (std::min(bank->pSpaceControlCurrent->throttle[ControlSS::ROLLNEG ], 1.f) * bank->pSpaceControlCurrent->thrust[ControlSS::ROLLNEG ])
            );            
            
            // zero the control component's input information after use.
            bank->pSpaceControlCurrent->zeroInputs();
        }
    }    
}
