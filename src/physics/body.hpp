//
// Created by Washington on 12/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_BODY_HPP
#define ENGINE3D_SRC_PHYSICS_BODY_HPP

#include <glm/gtc/quaternion.hpp>
#include "bounds.hpp"
#include "fixture.hpp"

#include <memory>

class World;

class Body {
public:
    Body(const glm::vec3 &center, const glm::vec3 &extents);

    void Move(const glm::vec3 &step);
    void Jump(float impulse);

    [[nodiscard]] bool IsStatic() const;
    void SetStatic(bool isStatic);

    [[nodiscard]] const Bounds &GetBounds() const;

    [[nodiscard]] const glm::vec3 &GetPosition() const;
    void SetPosition(const glm::vec3 &position);

    [[nodiscard]] const glm::vec3 &GetVelocity() const;
    void SetVelocity(const glm::vec3 &velocity);

    [[nodiscard]] bool IsGrounded() const;

    friend class World;

private:
    bool mStatic{false};
    Bounds mBounds;
    glm::vec3 mPosition{0.0f};
    glm::vec3 mVelocity{0.0f};
    bool mGrounded{false};
};

#endif //ENGINE3D_SRC_PHYSICS_BODY_HPP
