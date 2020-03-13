//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_RAY_HPP
#define ENGINE3D_SRC_PHYSICS_RAY_HPP

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 point1{0.0f};
    glm::vec3 point2{0.0f};
    float maxFraction{1.0f};
};


#endif //ENGINE3D_SRC_PHYSICS_RAY_HPP
