//
// Created by Washington on 12/03/2020.
//

#include "body.hpp"

Body::Body(const glm::vec3 &center, const glm::vec3 &extents) :
    mBounds{glm::vec3(center - extents), glm::vec3(center + extents)} {}

void Body::Move(const glm::vec3 &step) {
    mVelocity.x = step.x;
    mVelocity.z = step.z;
}

void Body::Jump(float impulse) {
    mVelocity.y = impulse;
}

bool Body::IsStatic() const {
    return mStatic;
}

void Body::SetStatic(bool isStatic) {
    mStatic = isStatic;
}

const Bounds &Body::GetBounds() const {
    return mBounds;
}

const glm::vec3 &Body::GetPosition() const {
    return mPosition;
}

void Body::SetPosition(const glm::vec3 &position) {
    mPosition = position;
}

const glm::vec3 &Body::GetVelocity() const {
    return mVelocity;
}

void Body::SetVelocity(const glm::vec3 &velocity) {
    mVelocity = velocity;
}

bool Body::IsGrounded() const {
    return mGrounded;
}
