#include "Control_SS.h"
#include <algorithm>

#include "ControlTypes.h"
#include "Functions.h"

using namespace DualityEngine;

System_Control_SS::System_Control_SS(Bank * bank)
                         : System<System_Control_SS>(bank, "Space Ship Control System", 0)
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
            
            float breakingVectorComponentMultiplier = 0.f;
            glm::vec3 vectorVel{0,0,0};
            float ctrlAxisDotProducts[3]{0.f,0.f,0.f};  
            
            // if linear breaking input is non-zero
            if (bank->pSpaceControlCurrent->throttle[ControlSS::LINBRAKE]){ // apply braking vector to throttle values...
                // store the entity's current velocity                
                vectorVel = bank->getLinearVelocPtr(bank->activeControlID)->velLinear;                    

                if(vectorVel.x || vectorVel.y || vectorVel.z){
                    // if velocity is too small the braking algorithm produces floating point errors, so a deadzone was introduced. Will zero velocity if already below a threshold magnitude.
                    if (glm::length(vectorVel) < DUA_CONTROLSS_LINEAR_BRAKING_DEADZONE) {
                        bank->getLinearVelocPtr(bank->activeControlID)->zero();
                    } else {
                        // calculate dot products between velocity vector and thrust vectors
                        for (uint i = 0; i < 3; ++i){
                            ctrlAxisDotProducts[i] = glm::dot(vectorVel, bank->pSpaceControlCurrent->currentAxes[i]);
                        }
                        // find which dot product largest in magnitude in order to scale that thruster's throttle to full (scaling others by same factor, resulting in less-than-full throttles)
                        if (fabs(ctrlAxisDotProducts[0]) >= fabs(ctrlAxisDotProducts[1])){
                            if (fabs(ctrlAxisDotProducts[0]) >= fabs(ctrlAxisDotProducts[2])){
                                breakingVectorComponentMultiplier = fabs(1.f / ctrlAxisDotProducts[0]);
                            }else{
                                breakingVectorComponentMultiplier = fabs(1.f / ctrlAxisDotProducts[2]);
                            }                    
                        }else if (fabs(ctrlAxisDotProducts[1]) >= fabs(ctrlAxisDotProducts[2])){
                            breakingVectorComponentMultiplier = fabs(1.f / ctrlAxisDotProducts[1]);
                        }else{
                            breakingVectorComponentMultiplier = fabs(1.f / ctrlAxisDotProducts[2]);
                        }

                        // add the appropriate values to the control component's throttle array (allowing no value to pass 1)
                        for (uint i = 0; i < 3; ++i){
                            // only break in a direction if the user isn't trying to thrust that direction!
                            if (! bank->pSpaceControlCurrent->throttle[i * 2 + int(ctrlAxisDotProducts[i] <= 0)]){
                                    // apply braking thrust to the appropriate directions
                                    int opposingThrustDecider = int(ctrlAxisDotProducts[i] > 0);
                                    int thrustMostOppositeVelocity = i * 2 + opposingThrustDecider;
                                    bank->pSpaceControlCurrent->throttle[thrustMostOppositeVelocity] += fabs(ctrlAxisDotProducts[i]) * breakingVectorComponentMultiplier
                                            * (std::min( (ctrlAxisDotProducts[i] * (-1 + 2 * opposingThrustDecider) ) / bank->pSpaceControlCurrent->thrust[thrustMostOppositeVelocity], 1.f))
                                            * bank->pSpaceControlCurrent->throttle[ControlSS::LINBRAKE];
                            }
                        }
                    }
                }
            }
            
            // if angular breaking input is non-zero
            if (bank->pSpaceControlCurrent->throttle[ControlSS::ANGBRAKE]) { // apply braking vector to throttle values...
                // store the entity's current angular velocity
                vectorVel = bank->getAngularVelocPtr(bank->activeControlID)->velAngular;                    

                if(vectorVel.x || vectorVel.y || vectorVel.z){
                    // if velocity is too small the braking algorithm produces floating point errors, so a deadzone was introduced. Will zero velocity if already below a threshold magnitude.
                    if (glm::length(vectorVel) < DUA_CONTROLSS_ANGULAR_BRAKING_DEADZONE) {
                        bank->getAngularVelocPtr(bank->activeControlID)->zero();
                    } else {
                        // find which angveloc component is largest in order to scale that torques's throttle to full (scaling others by same factor, resulting in less-than-full throttles)
                        if (fabs(vectorVel[0]) >= fabs(vectorVel[1])){
                            if (fabs(vectorVel[0]) >= fabs(vectorVel[2])){
                                breakingVectorComponentMultiplier = fabs(1.f / vectorVel[0]);
                            }else{
                                breakingVectorComponentMultiplier = fabs(1.f / vectorVel[2]);
                            }                    
                        }else if (fabs(vectorVel[1]) >= fabs(vectorVel[2])){
                            breakingVectorComponentMultiplier = fabs(1.f / vectorVel[1]);
                        }else{
                            breakingVectorComponentMultiplier = fabs(1.f / vectorVel[2]);
                        }
                        // add the appropriate values to the control component's throttle array (allowing no value to pass 1)
                        for (uint i = 0; i < 3; ++i){
                            // only break in a direction if the user isn't trying to thrust that direction!
                            if (! bank->pSpaceControlCurrent->throttle[i * 2 + 6 + int(vectorVel[i] <= 0)]){
                                    int opposingThrustDecider = int(vectorVel[i] > 0);
                                    int thrustMostOppositeVelocity = i * 2 + 6 + opposingThrustDecider;
                                    bank->pSpaceControlCurrent->throttle[thrustMostOppositeVelocity] += fabs(vectorVel[i]) * breakingVectorComponentMultiplier
                                            * (std::min( (vectorVel[i] * (-1 + 2 * opposingThrustDecider) ) / bank->pSpaceControlCurrent->thrust[thrustMostOppositeVelocity], 1.f))
                                            * bank->pSpaceControlCurrent->throttle[ControlSS::ANGBRAKE];
                            }
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
