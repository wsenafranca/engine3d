//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_NODE_HPP
#define ENGINE3D_SRC_NODE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../base/object.hpp"
#include "light.hpp"
#include "../graphics/mesh.hpp"
#include "camera.hpp"
#include <list>
#include "../graphics/meshbatch.hpp"
#include "bone.hpp"

struct Node {
    std::string name;
    Light* light{nullptr};
    std::list<Mesh*> meshes;
    Camera* camera{nullptr};
    Node* parent{nullptr};
    std::list<Node*> children;
    Bone* bone{nullptr};
    glm::mat4 matrix{1.0f};
};

#endif //ENGINE3D_SRC_NODE_HPP
