//
// Created by Washington on 13/03/2020.
//

#include "rigidbody.hpp"

std::shared_ptr<RigidBody> RigidBody::Create(Body *body) {
    auto rigidBody = std::make_shared<RigidBody>();
    rigidBody->mBody = body;
    return rigidBody;
}

void RigidBody::Update(float dt) {
    auto target = mTarget.lock();
    target->SetLocalTransform(mBody->GetTransform());
}

void RigidBody::Move(const glm::vec3 &step) {
     mBody->Translate(step);
}

bool RigidBody::IsGround() const {
    return mGround;
}

bool RigidBody::IsFalling() const {
    return mFalling;
}

bool RigidBody::IsJumping() const {
    return mJumping;
}

const glm::vec3 &RigidBody::GetVelocity() const {
    return mVelocity;
}

Body *RigidBody::GetBody() const {
    return mBody;
}
