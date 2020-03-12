//
// Created by Washington on 11/03/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_COMPONENT_HPP
#define ENGINE3D_SRC_COMPONENTS_COMPONENT_HPP

#include <string>
#include <memory>

class Node;

class Component {
public:
    virtual ~Component() = default;
    virtual void Update(float dt) = 0;

    [[nodiscard]] const std::weak_ptr<Node> &GetTarget() const;
    virtual void SetTarget(const std::weak_ptr<Node> &target);

protected:
    std::weak_ptr<Node> mTarget;
};


#endif //ENGINE3D_SRC_COMPONENTS_COMPONENT_HPP
