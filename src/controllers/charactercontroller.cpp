//
// Created by Washington on 27/02/2020.
//

#include "charactercontroller.hpp"

void CharacterController::Move(const glm::vec3 &motion) {
    if(!mMoveEnabled) return;
    mVelocity = motion;
}

void CharacterController::Yaw(float angle) {
    mYaw += angle;
}

void CharacterController::Pitch(float angle) {

}

void CharacterController::Roll(float angle) {

}

void CharacterController::Jump(float amount) {
    mVelocity.y = amount;
}

void CharacterController::Update(float dt) {
    mStateTime += dt;
    mVelocity.y -= 20.0f*dt;
    mPosition += mVelocity*dt;
    mPosition.y = glm::max(mPosition.y, 0.0f);
    mGrounded = mPosition.y < 0.1f;
    mJumping = !mGrounded && mVelocity.y > 0.0f;
    mFalling = !mGrounded && mVelocity.y < 0.0f;
    //float a = glm::clamp(mStateTime*6.0f, 0.0f, 1.0f);
    //mRotation = glm::normalize(glm::slerp(mSourceRotation, mTargetRotation, a));
}

const glm::vec3 &CharacterController::GetPosition() const {
    return mPosition;
}

void CharacterController::SetPosition(const glm::vec3 &position) {
    mPosition = position;
}

glm::quat CharacterController::GetRotation() const {
    auto qYaw = glm::angleAxis(mYaw, glm::vec3(0.0f, 1.0f, 0.0f));
    return qYaw;
}

void CharacterController::SetRotation(const glm::quat &rotation) {
    mRotation = rotation;
    /*
    if(mTargetRotation != rotation) {
        mTargetRotation = rotation;
        mSourceRotation = mRotation;
        mStateTime = 0.0f;
    }
     */
}

const glm::vec3 &CharacterController::GetVelocity() const {
    return mVelocity;
}

bool CharacterController::IsGrounded() const {
    return mGrounded;
}

bool CharacterController::IsMoving() const {
    return glm::abs(mVelocity.x) > glm::epsilon<float>() || glm::abs(mVelocity.z) > glm::epsilon<float>();
}

bool CharacterController::IsJumping() const {
    return mJumping;
}

bool CharacterController::IsFalling() const {
    return mFalling;
}

bool CharacterController::IsMoveEnabled() const {
    return mMoveEnabled;
}

void CharacterController::SetMoveEnabled(bool moveEnabled) {
    mMoveEnabled = moveEnabled;
}
