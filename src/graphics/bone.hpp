//
// Created by Washington on 25/02/2020.
//

#ifndef ENGINE3D_SRC_BONE_HPP
#define ENGINE3D_SRC_BONE_HPP

#include <string>
#include <glm/glm.hpp>

class Node;

struct Bone {
    std::string name;
    uint32_t index;
    glm::mat4 offsetMatrix{1.0f};
    std::shared_ptr<Node> skeleton;
    std::list< std::shared_ptr<Node> > joints;
};

#endif //ENGINE3D_SRC_BONE_HPP
