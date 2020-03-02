//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_BONE_HPP
#define ENGINE3D_SRC_BONE_HPP

#include <string>
#include <glm/glm.hpp>

struct Bone {
    std::string name;
    uint32_t index;
    glm::mat4 offsetMatrix;
};

#endif //ENGINE3D_SRC_BONE_HPP
