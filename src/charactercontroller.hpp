//
// Created by Washington on 27/02/2020.
//

#ifndef ENGINE3D_SRC_CHARACTERCONTROLLER_HPP
#define ENGINE3D_SRC_CHARACTERCONTROLLER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class CharacterController {
public:
    void Move(const glm::vec3& motion);
    void Yaw(float angle);
    void Pitch(float angle);
    void Roll(float angle);
    void Jump(float amount);
    void Update(float dt);

    [[nodiscard]] const glm::vec3 &GetPosition() const;
    void SetPosition(const glm::vec3 &position);

    [[nodiscard]] glm::quat GetRotation() const;
    void SetRotation(const glm::quat &rotation);

    [[nodiscard]] const glm::vec3 &GetVelocity() const;
    [[nodiscard]] bool IsGrounded() const;
    [[nodiscard]] bool IsMoving() const;
    [[nodiscard]] bool IsJumping() const;
    [[nodiscard]] bool IsFalling() const;

    [[nodiscard]] bool IsMoveEnabled() const;
    void SetMoveEnabled(bool moveEnabled);

    float mYaw{0.0f};
private:
    glm::vec3 mPosition{0.0f};
    glm::quat mRotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 mVelocity{0.0f};
    glm::quat mRoll{1.0f, 0.0f, 0.0f, 0.0f};
    glm::quat mPitch{1.0f, 0.0f, 0.0f, 0.0f};
    float mStateTime{0.0f};
    bool mGrounded{false};
    bool mJumping{false};
    bool mFalling{false};
    bool mMoveEnabled{true};
};


#endif //ENGINE3D_SRC_CHARACTERCONTROLLER_HPP
