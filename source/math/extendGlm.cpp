//
// Created by adayoldbagel on 2/3/16.
//

#include "extendGlm.h"

namespace DualityEngine {
    void vec3Interpolate(glm::vec3& lhs, glm::vec3& rhs, glm::vec3& result, float interp) {
        glm::vec3 diff = rhs - lhs;
        result = lhs + interp * diff;
    }
}