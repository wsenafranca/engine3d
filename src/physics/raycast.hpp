//
// Created by Washington on 14/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_RAYCAST_HPP
#define ENGINE3D_SRC_PHYSICS_RAYCAST_HPP

#include <glm/glm.hpp>
#include "body.hpp"

struct RayCastHit {
    glm::vec3 normal{0.0f};
    glm::vec3 point{0.0f};
    float fraction{0.0f};
    Body *body{nullptr};
};

#endif //ENGINE3D_SRC_PHYSICS_RAYCAST_HPP
