//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_RIGIDBODY_HPP
#define ENGINE3D_SRC_COMPONENTS_RIGIDBODY_HPP

#include "component.hpp"
#include <physics/body.hpp>
#include <graphics/node.hpp>

class RigidBody : public Component {
public:
    explicit RigidBody(std::shared_ptr<Body> body);

    void Update(float dt) override;

    const std::shared_ptr<Body> &GetBody() const;

private:
    std::shared_ptr<Body> mBody;
};


#endif //ENGINE3D_SRC_COMPONENTS_RIGIDBODY_HPP
