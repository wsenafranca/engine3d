//
// Created by Washington on 15/03/2020.
//

#include <glm/gtc/type_ptr.hpp>
#include "character.hpp"
#include "physics.hpp"

Character::~Character() {
    Physics::DestroyCharacter(this);
}

void Character::Move(const glm::vec3 &walkDirection) {
    mCharacterController->setWalkDirection(btVector3(walkDirection.x, walkDirection.y, walkDirection.z));
}

void Character::MoveForTimeInterval(const glm::vec3 &velocity, float timeInternal) {
    mCharacterController->setVelocityForTimeInterval(btVector3(velocity.x, velocity.y, velocity.z), timeInternal);
}

void Character::Translate(const glm::vec3 &translation) {
    auto t = mCharacterController->getGhostObject()->getWorldTransform();
    auto pos = btVector3(translation.x, translation.y, translation.z);
    t.setOrigin(t.getOrigin() + pos);
    mCharacterController->getGhostObject()->setWorldTransform(t);
}

void Character::Rotate(const glm::quat &rotation) {
    auto t = mCharacterController->getGhostObject()->getWorldTransform();
    auto rot = btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w);
    t.setRotation(t.getRotation()*rot);
    mCharacterController->getGhostObject()->setWorldTransform(t);
}

void Character::SetPosition(const glm::vec3 &origin) {
    mCharacterController->warp(btVector3(origin.x, origin.y, origin.z));
}

glm::vec3 Character::GetPosition() const {
    return glm::make_vec3(&mCharacterController->getGhostObject()->getWorldTransform().getOrigin().x());
}

void Character::SetRotation(const glm::quat &rotation) {
    mCharacterController->getGhostObject()->getWorldTransform().setRotation(
            btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
}

glm::quat Character::GetRotation() const {
    auto rot = mCharacterController->getGhostObject()->getWorldTransform().getRotation();
    return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
}

void Character::SetAngularVelocity(const glm::vec3 &velocity) {
    mCharacterController->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

glm::vec3 Character::GetAngularVelocity() const {
    return glm::make_vec3(&mCharacterController->getAngularVelocity().x());
}

void Character::SetLinearVelocity(const glm::vec3 &velocity) {
    mCharacterController->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

glm::vec3 Character::GetLinearVelocity() const {
    return glm::make_vec3(&mCharacterController->getLinearVelocity().x());
}

void Character::SetStepHeight(float h) {
    mCharacterController->setStepHeight(h);
}

float Character::GetStepHeight() const {
    return mCharacterController->getStepHeight();
}

void Character::SetFallSpeed(float fallSpeed) {
    mCharacterController->setFallSpeed(fallSpeed);
}

float Character::GetFallSpeed() const {
    return mCharacterController->getFallSpeed();
}

void Character::SetJumpSpeed(float jumpSpeed) {
    mCharacterController->setJumpSpeed(jumpSpeed);
}

float Character::GetJumpSpeed() const {
    return mCharacterController->getJumpSpeed();
}

void Character::SetMaxJumpHeight(float maxJumpHeight) {
    mCharacterController->setMaxJumpHeight(maxJumpHeight);
}

void Character::Jump(float impulse) {
    mCharacterController->jump(btVector3(0, impulse, 0));
}

bool Character::CanJump() const {
    return mCharacterController->canJump();
}

void Character::ApplyImpulse(const glm::vec3 &v) {
    mCharacterController->applyImpulse(btVector3(v.x, v.y, v.z));
}

void Character::SetGravity(const glm::vec3 &gravity) {
    mCharacterController->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

glm::vec3 Character::GetGravity() const {
    return glm::make_vec3(&mCharacterController->getGravity().x());
}

void Character::SetMaxSlope(float slopeRadians) {
    mCharacterController->setMaxSlope(slopeRadians);
}

float Character::GetMaxSlope() const {
    return mCharacterController->getMaxSlope();
}

void Character::SetMaxPenetrationDepth(float d) {
    mCharacterController->setMaxPenetrationDepth(d);
}

float Character::GetMaxPenetrationDepth() const {
    return mCharacterController->getMaxPenetrationDepth();
}

bool Character::OnGround() const {
    return mCharacterController->onGround();
}

bool Character::IsWalking() const {
    btVector3 vel = mCharacterController->getLinearVelocity();
    vel.setY(0.0f);
    return vel.length2() > 0.0f;
}

bool Character::IsJumping() const {
    btVector3 vel = mCharacterController->getLinearVelocity();
    return !mCharacterController->onGround() && vel.y() > 0.0f;
}

Transform Character::GetTransform() const {
    auto transform = mCharacterController->getGhostObject()->getWorldTransform();
    return Transform(
            mOffset + glm::vec3(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()),
            glm::quat(transform.getRotation().w(), transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z()),
            glm::vec3(1.0f));
}

const glm::vec3 &Character::GetOffset() const {
    return mOffset;
}

void Character::SetOffset(const glm::vec3 &offset) {
    mOffset = offset;
}
