//
// Created by Washington on 11/03/2020.
//

#ifndef ENGINE3D_SRC_GRAPHICS_SKELETON_HPP
#define ENGINE3D_SRC_GRAPHICS_SKELETON_HPP

#include <list>

class Node;

struct Skeleton {
    std::string name;
    std::list< std::weak_ptr<Node> > joints;
    std::weak_ptr<Node> root;
};


#endif //ENGINE3D_SRC_GRAPHICS_SKELETON_HPP
