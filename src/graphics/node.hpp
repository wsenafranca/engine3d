//
// Created by Washington on 21/02/2020.
//

#ifndef ENGINE3D_SRC_NODE_HPP
#define ENGINE3D_SRC_NODE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../base/object.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include <list>
#include "meshbatch.hpp"
#include "bone.hpp"

struct Node {
    std::string name;
    std::shared_ptr<Light> light;
    std::vector< std::shared_ptr<Mesh> > meshes;
    std::shared_ptr<Camera> camera;
    Node* parent{nullptr};
    std::list< std::shared_ptr<Node> > children;
    std::shared_ptr<Bone> bone;
};

#endif //ENGINE3D_SRC_NODE_HPP
