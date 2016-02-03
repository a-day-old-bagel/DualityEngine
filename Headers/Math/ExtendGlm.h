//
// Created by adayoldbagel on 2/3/16.
//

#ifndef DUALITYENGINE_EXTENDGLM_H
#define DUALITYENGINE_EXTENDGLM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DualityEngine {
    void vec3Interpolate(glm::vec3& lhs, glm::vec3& rhs, glm::vec3& result, float interp);
}

#endif //DUALITYENGINE_EXTENDGLM_H
