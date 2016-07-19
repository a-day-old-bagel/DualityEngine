#include "PhysCollide.h"

namespace DualityEngine {

    System_PhysCollide::System_PhysCollide(Bank* bank)
            : System<System_PhysCollide>(bank, "Physics Collision System", 1) {
        requiredComponents.at(0) = COLLISION | POSITION;
    }

    bool System_PhysCollide::onInit(std::stringstream& output) {
        return true;
    }

    void System_PhysCollide::onTick() {

    }

    void System_PhysCollide::onClean() {

    }
}


