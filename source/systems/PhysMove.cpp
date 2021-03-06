#include "PhysMove.h"

namespace DualityEngine {

    System_PhysMove::System_PhysMove(Bank* bank)
            : System<System_PhysMove>(bank, "Physics Movement System", 2) {
        requiredComponents.at(0) = POSITION | LINVELOC;
        requiredComponents.at(1) = ORIENTATION | ANGVELOC;
    }

    bool System_PhysMove::onInit(std::stringstream& output) {
        lastTime = bank->getTime();
        return true;
    }

    void System_PhysMove::onTick() {
        DUA_uint32 currentTime = bank->getTime();
        DUA_uint32 delta = (currentTime - lastTime);
        lastTime = currentTime;
        for (unsigned i = 0; i < registeredIDs[0].size(); ++i) {
            if (bank->getState(registeredIDs[0][i]) & ACTIVE) {
                bank->getPositionPtr(registeredIDs[0][i])->translate(
                        bank->getLinearVelocPtr(registeredIDs[0][i])->velLinear.x * delta,
                        bank->getLinearVelocPtr(registeredIDs[0][i])->velLinear.y * delta,
                        bank->getLinearVelocPtr(registeredIDs[0][i])->velLinear.z * delta,
                        currentTime
                );
                bank->stateOn(registeredIDs[0][i], RECALCVIEWMAT);
            }
        }
        for (unsigned i = 0; i < registeredIDs[1].size(); ++i) {
            if (bank->getState(registeredIDs[1][i]) & ACTIVE) {
                bank->getOrientationPtr(registeredIDs[1][i])->rotate(
                        bank->getAngularVelocPtr(registeredIDs[1][i])->velAngular.x * delta,
                        bank->getAngularVelocPtr(registeredIDs[1][i])->velAngular.y * delta,
                        bank->getAngularVelocPtr(registeredIDs[1][i])->velAngular.z * delta
                );
                bank->stateOn(registeredIDs[1][i], RECALCVIEWMAT);
            }
        }
        SDL_Delay(1);
    }

    void System_PhysMove::onClean() {

    }
}



