//
// Created by Washington on 13/03/2020.
//

#ifndef ENGINE3D_SRC_COMPONENTS_RIGIDBODY_HPP
#define ENGINE3D_SRC_COMPONENTS_RIGIDBODY_HPP

#include <physics/body.hpp>
#include <graphics/node.hpp>
#include "component.hpp"

class RigidBody : public Component {
public:
    static std::shared_ptr<RigidBody> Create(Body *body);

    void Update(float dt) override;

    void Move(const glm::vec3 &step);

    [[nodiscard]] bool IsGround() const;
    [[nodiscard]] bool IsFalling() const;
    [[nodiscard]] bool IsJumping() const;
    [[nodiscard]] const glm::vec3 &GetVelocity() const;
    [[nodiscard]] Body *GetBody() const;

private:
    Body *mBody{nullptr};
    bool mGround{false};
    bool mFalling{false};
    bool mJumping{false};
    glm::vec3 mVelocity{0.0f};
};


#endif //ENGINE3D_SRC_COMPONENTS_RIGIDBODY_HPP
