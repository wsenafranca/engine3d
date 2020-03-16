//
// Created by Washington on 15/03/2020.
//

#ifndef ENGINE3D_SRC_PHYSICS_CHARACTER_HPP
#define ENGINE3D_SRC_PHYSICS_CHARACTER_HPP

#include <btBulletDynamicsCommon.h>
#include <math/transform.hpp>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class Character {
public:
    ~Character();

    void Move(const glm::vec3 & walkDirection);
    void MoveForTimeInterval(const glm::vec3 &velocity, float timeInternal);

    void Translate(const glm::vec3 &translation);
    void Rotate(const glm::quat &rotation);

    void SetPosition(const glm::vec3 &origin);
    [[nodiscard]] glm::vec3 GetPosition() const;

    void SetRotation(const glm::quat &rotation);
    [[nodiscard]] glm::quat GetRotation() const;

    void SetAngularVelocity(const glm::vec3 &velocity);
    [[nodiscard]] glm::vec3 GetAngularVelocity() const;

    void SetLinearVelocity(const glm::vec3 &velocity);
    [[nodiscard]] glm::vec3 GetLinearVelocity() const;

    void SetStepHeight(float h);
    [[nodiscard]] float GetStepHeight() const;

    void SetFallSpeed(float fallSpeed);
    [[nodiscard]] float GetFallSpeed() const;

    void SetJumpSpeed(float jumpSpeed);
    [[nodiscard]] float GetJumpSpeed() const;

    void SetMaxJumpHeight(float maxJumpHeight);

    void Jump(float impulse = 0.0f);
    [[nodiscard]] bool CanJump() const;

    void ApplyImpulse(const glm::vec3 &v);

    void SetGravity(const glm::vec3 &gravity);
    [[nodiscard]] glm::vec3 GetGravity() const;

    void SetMaxSlope(float slopeRadians);
    [[nodiscard]] float GetMaxSlope() const;

    void SetMaxPenetrationDepth(float d);
    [[nodiscard]] float GetMaxPenetrationDepth() const;

    [[nodiscard]] bool OnGround() const;
    [[nodiscard]] bool IsWalking() const;
    [[nodiscard]] bool IsJumping() const;

    [[nodiscard]] Transform GetTransform() const;

    const glm::vec3 &GetOffset() const;

    void SetOffset(const glm::vec3 &offset);

    friend class TPhysics;
private:
    btKinematicCharacterController *mCharacterController;
    glm::vec3 mOffset{0.0f, 0.0f, 0.0f};
};


#endif //ENGINE3D_SRC_PHYSICS_CHARACTER_HPP
